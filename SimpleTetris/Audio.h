#pragma once
#include <Fmod/fmod.hpp>
//#include <string.h> // C 헤더 파일
#include <string>
#include <vector>
#include <unordered_map>

namespace hady 
{
	class Audio
	{
	public:
		Audio();
		~Audio();
	
	public:
		void systemCreateAndInit();
		// 초록색은 const 참조형으로 받자!
		void createSound(const std::string& fileName, FMOD_MODE fMode, const std::string& soundKey);

		// 해시를 쓰기 때문에 키(key)를 입력한다.
		void playSound(const std::string& soundKey);
		
		void stopSound(const std::string& soundKey);

		void release();

	private:
		FMOD::System*							_system{};

		std::vector<FMOD::Sound*>				_vSound{};

		std::vector<FMOD::Channel*>				_vChannel{};

		std::unordered_map<std::string, size_t> _soundMap{};
	};




}