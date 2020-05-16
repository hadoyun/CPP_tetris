#pragma once

#include "GraphicalWindow.h"
//#include <queue>
#include <deque>
#include "CheapTimer.h"

namespace hady
{
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
		SimpleTetris(int32 width, int32 height);
		~SimpleTetris();

	public:
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

	public:
		void updateNextblockQueue();

	public:
		void setTimerInterval(int32 interval);
		int32 getTimerInterval() const;
		bool tickGameSpeedTimer() const;

	public:
		void updateGameLevel();

	public:
		uint32 getCurrScore() const;
		uint32 getCurrLevel() const;
		uint32 getCurrLevelScore() const;

	public:
		bool isGameOver() const;

	public:
		void restartGame();

	private:
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
		void checkBingo();
		void changeBingoLineColor(int32 bingoedY);
		void clearBingoLine(int32 bingoedY);

	public:
		uint32 getComboCount() const;
		void addComboCount();
		bool getGameLevelUp() const;
		void resetGameLevelUp();

	public:
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
		//보드의 각 항목은 eblocks의 한 항목의 값이다.
		uint8					_board[uint32(kBoardSize.y + 1)][uint32(kBoardSize.x)]{};
		//

	private:
		// 밀리초 = ms
		// 마이크로초 = us (그리스어 뮤랑 제일 닮아서)
		int32					_gameSpeed{ 1000 };
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
		uint32					_currLevelScore{};
		uint32					_scoreForNextLevel{ 10 };


	private:
		bool _isGameOver{ false };

		uint8 _blockspwansCounts[uint32(EBlockType::MAX)][uint32(EBlockType::MAX)]{};

		mutable bool			_pause{ false };

	private:
		bool					_isBingo{ false };
		uint32					_comboCount{};
		mutable bool			_isLevelup{ false };

	private:
		CheapTimer				_bingoTimer{};
		//double ended queue
		std::deque<uint32>		_bingoLines{};
	};
}
