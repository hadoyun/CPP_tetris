#pragma once

#include "GraphicalWindow.h"
#include <vector>
#include <deque>

namespace fs
{
	struct BlockContainer
	{
		uint8 data[4][4]{};

		void set(
			uint8 _00, uint8 _10, uint8 _20, uint8 _30,
			uint8 _01, uint8 _11, uint8 _21, uint8 _31,
			uint8 _02, uint8 _12, uint8 _22, uint8 _32,
			uint8 _03, uint8 _13, uint8 _23, uint8 _33
		)
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
		none = 0,
		used,
		I,
		T,
		O,
		L,
		InvL,
		Z,
		InvZ,
		MAX,
	};

	enum class EDirection
	{
		N = 0,
		W,
		S,
		E,
		MAX,
	};

	class SimpleTetris final : public IGraphicalWindow
	{
	public:
		SimpleTetris(int32 width, int32 height);
		~SimpleTetris();

	public:
		virtual void create(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc) override;
		
		void createBlock(EBlockType _eBlockType, const Color& color, uint8 alpha = 255);

	public:
		void drawBoard(const Position2& position, const Color& borderColor, const Color& boardColor);
		bool move(EDirection _eDirection);
		void rotate();

		const Position2& getCurrPos() const;

		bool getRotablePosition(EDirection _eNextdir, fs::Position2& outPosition) const;

		EBlockType getCurrentBlockType() const;

		void setCurrentBlockType(EBlockType eBlockType);
		
	private:
		EBlockType getNextBlockType() const;
		

	private:
		void drawBlockUnitToImage(EBlockType _eBlockType, const Position2& position, const Color& color, uint8 alpha = 255);
		
		void drawBlockToBoard(EBlockType _eBlockType, const Position2& position, EDirection _eDirect, bool bErase = false);

		void drawBlockToScreen(EBlockType _eBlockType, const Position2& position, EDirection _eDirect);

	private:
		bool canDrawBlock(EBlockType _eBlockType, const Position2& position, EDirection _eDirect) const;
		
	public:
		void setTimerInterval(int32 interval);
		int32 getTimerInterval() const;
		bool tickTimer() const; 

	public:
		void updateNextBlockQueue();

		uint32 getScore();

		bool isGameOver() const;

	private:
		void checkBingo();

	public:
		static constexpr Size2	kBlockSize{ 30, 30 }; // 블록 하나의 크기 
		static constexpr float	kBlockBorder{ 4 }; // 블록 하나의 둘레 
		static constexpr Size2	kBoardSize{ 10, 20 }; // board의 크기 
		static constexpr Size2	kBoardSizePixel{ kBlockSize * kBoardSize }; //보드의 전체 픽셀 지정
		static constexpr int32	kBlockContainerSize{ 4 }; 

		static constexpr int32	kTimerIntervalMin { 100 }; // 블록 하나의 둘레 
		static constexpr int32  kNextBlockQueueMin{ 5 };
		static constexpr int32  kNextBlockQueueMax{ 20 };
	// ii == image index
	private:
		uint32					_iiBlocks[(uint32)EBlockType::MAX]{};

		Position2				_currPos{};
		EDirection				_currdir{ EDirection::N };
		EBlockType				_currBlockType{ EBlockType::I };
	private:

		uint8					_aaBoard[(uint32)kBlockSize.y][(uint32)kBlockSize.x]{};
		BlockContainer			_blocks[(int)fs::EBlockType::MAX][(int)EDirection::MAX]{};

	private:
		int32					_timerInterval{ 1000 }; // 1000 ms == 1 s
		mutable std::chrono::steady_clock::time_point _prevTime{};

	private:
		std::deque<EBlockType>	_nextBlockQueue{};
	
	private:
		uint32					_score{};
		bool					_isGameOver{ false };
	};
}
