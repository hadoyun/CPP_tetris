#pragma once

#include "GraphicalWindow.h"
//#include <queue>
#include <deque>
#include "CheapTimer.h"

// include 경로와 lib경로는 다르게 설정되어있다.
//#include "../FMOD/fmod.hpp"
//#pragma comment(lib, "Lib/fmodL_vc.lib")

//포함 디렉토리 설정으로 인식성이 좋은 인클루드
// cpp용 헤더
#include <Fmod/fmod.hpp>
#include <SimpleTetris/Audio.h>

namespace hady
{
	// 코드의 단순화를 위해 다양한 블록을 담는 4*4 구조체
	struct BlockContainer
	{
		uint8 data[4][4]{};

		void set(uint8 _00, uint8 _10, uint8 _20, uint8 _30,
			uint8 _01, uint8 _11, uint8 _21, uint8 _31,
			uint8 _02, uint8 _12, uint8 _22, uint8 _32,
			uint8 _03, uint8 _13, uint8 _23, uint8 _33)
		{
			data[0][0] = _00;
			data[0][1] = _10;
			data[0][2] = _20;
			data[0][3] = _30;

			data[1][0] = _01;
			data[1][1] = _11;
			data[1][2] = _21;
			data[1][3] = _31;

			data[2][0] = _02;
			data[2][1] = _12;
			data[2][2] = _22;
			data[2][3] = _32;

			data[3][0] = _03;
			data[3][1] = _13;
			data[3][2] = _23;
			data[3][3] = _33;
		}
	};
	
	//블록을 종류를 담은 Enum
	enum class EBlockType
	{
		None,
		Used, // Gray
		I, // Blue
		T, // Purple
		O, // Yellow
		L, // Orange
		InvL, // Skyblue
		Z, // green
		S, // Pink
		Bingo,
		MAX
	};
	
	//블록의 방향을 알려주는 Enum
	enum class EDirection
	{
		N,
		W,
		S,
		E,
		MAX
	};
	
	class SimpleTetris final : public IGraphicalWindow
	{
	public:
		// 시작시 클레스 빙고 타이머 선언한다.
		SimpleTetris(int32 width, int32 height);
		~SimpleTetris();

	public:
		// ????
		virtual void set(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc) override;
		// 넣을 수 있는 보드 Y축의 최대값을 묻고, 가능하다면 반 투명한 블록을 그린다.
		void drawGuideBlock(Position2 boardOffset);

	public:
		// 판, 테두리, 현재 블록을 그린다.
		void drawBoard(const Position2& boardOffset, const Color& borderColor, const Color& boardColor);
		// board에 시각 효과를 높이기 위해 격자무늬 그리드를 그린다.
		void drawGrid(const Position2& startPosition);

	public:
		// 블록이 해당 방향으로 이동시킨 후 bool 변수를 리턴한다.
		bool move(EDirection eDirection);
		// _currDirt를 바꾸고 그 에 따라 블록을 바꾼다.
		void rotate(bool clockWise);

	private:
		// 회전이 가능한 지 묻고, 회전한 블록이 보드 밖으로 나가지 않게 한다.
		const bool getRotatablePosition(EDirection eNextDirection, hady::Position2& outPosition) const;

	public:
		// 현재의 위치를 리턴한다. (계산용)
		const Position2& getCurrPosition() const;

	public:
		// 디버깅용! 현재 블록에 바꾼 블록을 대입한다.
		void setCurrBlockType(EBlockType eBlockType);
		// 현재 블록 타입을 가져온다.
		EBlockType getCurrBlockType() const;

	private:
		// 나온 숫자를 _prevblock에 넣고, 만약 중복되었다면 다른 enum을 뽑는다.
		EBlockType getRandomBlockType() const;
		// 랜덤으로 블록의 enum을 뽑아 uint32_t로 만든다
		int32 getRandomBlockTypeInternal() const;

	public:
		// _nextNlockQueue에 블록 생성된 랜덤한 블록 타입을 넣는다.
		void updateNextblockQueue();

	public:
		// 디버깅용, _gameSpeed를 입력받은 값으로 만든다.
		void setTimerInterval(int32 interval);

		// _gameSpeed의 값을 가져온다.
		int32 getTimerInterval() const;
		// 진행된 시간이 게임 스피드보다 크거나 같다면, 과거 시간을 현재 시간으로 바꾼 후 true를 리턴한다.
		bool tickGameSpeedTimer() const;

	public:
		// 일정 경험치를 얻었다면 _currLevel을 업데이트한다.
		void updateGameLevel();

	public:
		// _CurrScore를 가져온다.
		uint32 getCurrScore() const;
		// _CurrLevel를 가져온다.
		uint32 getCurrLevel() const;
		// _CurrLevelScore를 가져온다.
		uint32 getCurrLevelScore() const;

	public:
		// _isGameOver를 리턴한다.
		bool isGameOver() const;
	
	// 게임 시작과 블록 리스폰
	public:
		// 게임을 재시작, 현재 레벨, 스코어, 경험치, 콤보 등을 초기화하고, 보드를 비운다.
		void restartGame();
		// 대기 Queue의 블록을 현재 블록타입으로 만들고, 현재 위치를 시작 위치로 만든다.
		void spawnNewBlock();
	private:
		// 게임시작시, 블록의 처음 포지션을 리턴해주는 함수
		Position2 getInitialBlockPosition() const;
	
	private:
		// _iiBlocks에 이미지를 대입하고, 블록 타입, 포지션, 색상, 알파에 따라 블록을 그린다.
		void createBlock(EBlockType eBlockType, const Color& color, uint8 alpha = 255);

	private:
		// _iiblocks에 저장되어있는 인덱스를 받아, 게임의 최소 단위인 사각형을 그린다.
		void drawBlockUnitToImage(EBlockType eBlockType, const Position2& position, const Color& color, uint8 alpha = 255);
		// 해당 위치에 해당 블록(eBlockType)이 들어갈 수 있는지 없는지 확인하고 있다면 _board에 해당 블록의 enum을 넣는다.
		void setBlockToBoard(EBlockType eBlockType, const Position2& position, EDirection eDirection, bool bErase = false);
		// eblockType의 _iiBlock[] 항목을 화면에 그린다.
		void drawBlockToScreen(EBlockType eBlockType, const Position2& position, EDirection eDirection);

	private:
		// 해당 타입의 방향의 블록이 해당포지션으로 갈 수 있는지 없는지 확인해서 bool을 리턴한다.
		bool canDrawBlock(EBlockType eBlockType, const Position2& position, EDirection eDirection) const;

	public:
		// 호출시 _pause 변수를 true / false로 변환(toggle)해주는 함수
		void togglePause() const;
		// getter for _pause
		bool getPause() const;

	private:
		// 빙고를 체크함
		void checkBingo();
		// 체크된 빙고 번을 매개 변수로 받아 특정색으로 바꿔줌
		void changeBingoLineColor(int32 bingoedY);
		// 빙고가 되었다면, Y축을 Y - 1축으로 바꾼다.(???)
		void clearBingoLine(int32 bingoedY);

	public:
		uint32 getComboCount() const;
		void addComboCount();

		// getter
		bool getGameLevelUp() const;
		// _exe를 리셋한다.
		void resetGameLevelUp();

	public:
		// GraphicalWindow에서 상속받은 uqdate
		// 빙고가 되서 사라지는 애니메이션 효과를 주기해 clearBingoLine() 함수를 호출한다.
		virtual bool update() override;

	public:
		// _fmodSystem에 노래를 생성한다.
		void createAudioObjects(const std::string& AssetDir);

		void stopSound(const std::string& soundKind);

	public:
		void holdBlock();

	public:
		static constexpr Size2	kBlockSize{ 30, 30 };
		static constexpr float	kBlockBorder{ 2 };

		static constexpr Size2	kBoardSize{ 10, 20 };
		static constexpr Size2	kBoardSizePixel{ kBlockSize * kBoardSize };

		static constexpr int32 kTimerIntervalMin{ 100 };

		static constexpr int32 kBlockContainerSize{ 4 };
		// next block queue
		static constexpr int32 kNextBlockQueueMinSize{ 5 };
		static constexpr int32 kNextBlockQueueMaxSize{ 20 };

	private:
		// 블록의 종류 10가지(Used / Bingo 포함) * 4방향
		BlockContainer			_blocks[(uint32)EBlockType::MAX][(uint32)EDirection::MAX]{};

	private:
		// ii == image의 index
		uint32					_iiBlocks[(uint32)EBlockType::MAX]{};

	private:
		// 코드의 단순화를 위한 의도적 버퍼 오버런!
		uint8					_boardExtra[kBlockContainerSize][uint32(kBoardSize.x)]{};
		// 보드의 각 항목은 eblocks의 한 항목의 값이다.
		uint8					_board[uint32(kBoardSize.y + 1)][uint32(kBoardSize.x)]{};

	private:
		// 밀리초 = ms
		// 마이크로초 = us (그리스어 뮤랑 제일 닮아서)
		int32					_gameSpeed{ 1400 };
		mutable std::chrono::steady_clock::time_point _prevTime{};

	private:
		//현재 포지션
		Position2				_currPosition{};
		// 현재 블록 타입(default I)
		EBlockType				_currBlockType{ EBlockType::I };
		// 현재 블록 방향(default N)
		EDirection				_currDirection{ EDirection::N };
		
	private:
		// 현재 레벨
		uint32					_currLevel{};
		// 현재 스코어
		uint32					_currScore{};
		// 현재 경험치
		uint32					_currExe{};
		// 레벨업을 위한 필요 경험치
		uint32					_ExeForLevelUp{ 10 };

	private:
		// 다음 블록을 나타내기 위한 블록 큐(의미상)
		std::deque<EBlockType> _nextBlockQueue{};
		// hold Block
		std::deque<EBlockType>	_holdBlock{};
		bool					_canHold{ true };
		EBlockType				_prevHoldBlock{};
	private:
		bool					_isGameOver{ false };

		mutable bool			_pause{ false };

	private:
		bool					_isBingo{ false };
		uint32					_comboCount{};
		mutable bool			_isLevelup{ false };

	private:
		mutable int32			_prevRandomBlockType{};

	private:
		CheapTimer				_bingoTimer{};
		//double ended queue
		std::deque<uint32>		_bingoLines{};

	private:
		Audio					_audio{};
	};
}