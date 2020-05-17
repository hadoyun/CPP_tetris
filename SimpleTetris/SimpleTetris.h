#pragma once

#include "GraphicalWindow.h"
//#include <queue>
#include <deque>
#include "CheapTimer.h"

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
		//
		virtual void set(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc) override;

		void drawGuideBlock(Position2 boardOffset);

	public:
		void drawBoard(const Position2& boardOffset, const Color& borderColor, const Color& boardColor);
		void drawGrid(const Position2& startPosition);

	public:
		bool move(EDirection eDirection);
		void rotate(bool clockWise);

	private:
		const bool getRotatablePosition(EDirection eNextDirection, hady::Position2& outPosition) const;

	public:
		const Position2& getCurrPosition() const;

	public:
		void setCurrBlockType(EBlockType eBlockType);
		EBlockType getCurrBlockType() const;

	private:
		EBlockType getRandomBlockType() const;
		int32 getRandomBlockTypeInternal() const;

	public:
		void updateNextblockQueue();

	public:
		void setTimerInterval(int32 interval);
		int32 getTimerInterval() const;
		bool tickGameSpeedTimer() const;

	public:
		void updateGameLevel();

	public:
		// _CurrScore를 가져온다.
		uint32 getCurrScore() const;
		// _CurrLevel를 가져온다.
		uint32 getCurrLevel() const;
		// _CurrLevelScore를 가져온다.
		uint32 getCurrLevelScore() const;

	public:
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

		void createBlock(EBlockType eBlockType, const Color& color, uint8 alpha = 255);

	private:
		void drawBlockUnitToImage(EBlockType eBlockType, const Position2& position, const Color& color, uint8 alpha = 255);
		void setBlockToBoard(EBlockType eBlockType, const Position2& position, EDirection eDirection, bool bErase = false);
		void drawBlockToScreen(EBlockType eBlockType, const Position2& position, EDirection eDirection);

	private:
		bool canDrawBlock(EBlockType eBlockType, const Position2& position, EDirection eDirection) const;

	public:
		void togglePause() const;

		bool getPause() const;

	private:
		//빙고를 체크함
		void checkBingo();
		//체크된 빙고 줄을 받아서 (특정색으로 바꿔줌)
		void changeBingoLineColor(int32 bingoedY);
		void clearBingoLine(int32 bingoedY);

	public:
		uint32 getComboCount() const;
		void addComboCount();
		bool getGameLevelUp() const;
		void resetGameLevelUp();

	public:
		//GraphicalWindow에서 상속받은 uqdate
		//
		virtual bool update() override;

	public:
		static constexpr Size2	kBlockSize{ 30, 30 };
		static constexpr float	kBlockBorder{ 2 };
		static constexpr Size2	kBoardSize{ 10, 20 };
		static constexpr Size2	kBoardSizePixel{ kBlockSize * kBoardSize };

		static constexpr int32 kTimerIntervalMin{ 100 };
		static constexpr int32 kBlockContainerSize{ 4 };
		static constexpr int32 kNextBlockQueueMinSize{ 5 };
		static constexpr int32 kNextBlockQueueMaxSize{ 20 };

	private:
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
		Position2				_currPosition{};
		EBlockType				_currBlockType{ EBlockType::I };
		EDirection				_currDirection{ EDirection::N };

	private:
		std::deque<EBlockType> _nextBlockQueue{};

	private:
		uint32					_currLevel{};
		uint32					_currScore{};
		uint32					_currExe{};
		uint32					_scoreForNextLevel{ 10 };


	private:
		bool					_isGameOver{ false };

		uint8 _blockspwansCounts[uint32(EBlockType::MAX)][uint32(EBlockType::MAX)]{};

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
	};
}
