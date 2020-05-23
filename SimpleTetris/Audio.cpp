#include "Audio.h"
#include <assert.h>
#include <Windows.h>
#include <string.h>

// FMOD를 릴리스 하는 매크로
#define RELEASE_FMOD(s) if (s != nullptr) { s->release(); s = nullptr; }

hady::Audio::Audio()
{
	systemCreateAndInit();
}

hady::Audio::~Audio()
{
	release();
}

void hady::Audio::systemCreateAndInit()
{
	// 시스템 생성! 생성자를 private 선언함! 따라서 
	if (FMOD::System_Create(&_system) != FMOD_OK)
	{
		MessageBox(nullptr, TEXT("System Create Fail!"), TEXT("크리에이트 실패"), MB_ICONERROR);
	};
	// 생성된 시스템을 초기화 해준다...! 
	if (_system->init(32, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
	{
		MessageBox(nullptr, TEXT("System initialize fail!"), TEXT("초기화 실패"), MB_ICONERROR);
	}
}

//구조체가 쓰일때는 안에서 값을 바꿔주지 않는다면 const + &을 꼭 붙이자 !
void hady::Audio::createSound(const std::string& fileName, FMOD_MODE fMode, const std::string& soundKey)
{
	//배열은 정적 할당, 백터는 동적 할당,
	_vSound.emplace_back(nullptr);
	_vChannel.emplace_back(nullptr);

	// 함수의 리턴 자료형이 FMOD_RESULT이기 때문에 FMOD_OK로 비교해줘야한다.
	if (_system->createSound(fileName.c_str(), fMode, nullptr, &_vSound.back()) != FMOD_OK)
	{
		//사운드 생성 실패
		MessageBox(nullptr, TEXT("Sound Create fail!"), TEXT("음악 파일 생성 실패"), MB_ICONERROR);
		
		//실패했다면 위에서 nullptr값을 emplace_back() 해주었기 때문에 nullptr값을 빼줘야한다.
		_vSound.pop_back();

		// 매세지 박스가 실행되는 경우는 사운드 생성에 실패한 경우 임으로, _soundMap에 insert()하지 않게 한다.
		return;
	}
	
	//이곳에 내려오면 성공.
	//키와 값을 설정해주기
	_soundMap.insert(std::make_pair(soundKey, _vSound.size() - 1));
}

void hady::Audio::playSound(const std::string& soundKey)
{
	auto findKeyIter = _soundMap.find(soundKey);

	if (findKeyIter != _soundMap.end())
	{
		if (_system->playSound(_vSound[_soundMap.at(soundKey)], nullptr, false, &_vChannel[_soundMap.at(soundKey)]) != FMOD_OK)
		{
			MessageBox(nullptr, TEXT("Play sound fail!"), TEXT("음악 재생 실패"), MB_ICONERROR);
		}
	}
}

void hady::Audio::stopSound(const std::string& soundKey)
{
	// 자료형이 무엇인지 모르겠다면 auto에 넣어봐~
	auto channelIter = _soundMap.find(soundKey);

	if (channelIter != _soundMap.end())
	{
		if(_vChannel[_soundMap.at(soundKey)]->stop() != FMOD_OK)
		{
			MessageBox(nullptr, TEXT("Sound stop Fail"), TEXT("음악 중지 실패"), MB_ICONERROR);
		}
	}
}

void hady::Audio::release()
{
	//_vSound의 사운드 항목들을 하나하나 릴리스한다.
	for (auto& sound : _vSound)
	{
		RELEASE_FMOD(sound);
	}
	RELEASE_FMOD(_system);
}