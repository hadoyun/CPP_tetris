#include "SimpleTetris.h"

fs::SimpleTetris::SimpleTetris(int32 width, int32 height) : IGraphicalWindow(width, height)
{
	__noop;
}

fs::SimpleTetris::~SimpleTetris()
{
	__noop;
}

void fs::SimpleTetris::create(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc)
{
	createInternal(title, hInstance, windowProc);

	createBlock(EBlockType::used,	Color(100, 100, 100)); // board 블록을 그리는 함수 

	srand(std::chrono::steady_clock::now().time_since_epoch().count());


	//grey scale 전부다 무채색..! 
	// RGB가 서로 다를 수록 채도가 높아진다...!
	createBlock(EBlockType::I,		Color(255, 100, 100));
	createBlock(EBlockType::T,		Color(100, 240, 130));
	createBlock(EBlockType::O,		Color(100, 100, 255));
	createBlock(EBlockType::L,		Color(250, 250, 100));
	createBlock(EBlockType::InvL,	Color(100, 250, 250));
	createBlock(EBlockType::Z,		Color(240, 160,  50));
	createBlock(EBlockType::InvZ,	Color(255, 127, 255));


	// I형 블록 채우기
	{
		_blocks[(int)EBlockType::I][(int)EDirection::N].set(
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0
		);

		_blocks[(int)EBlockType::I][(int)EDirection::W].set(
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::I][(int)EDirection::S].set(
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0
		);

		_blocks[(int)EBlockType::I][(int)EDirection::E].set(
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}

	// T자형 블록
	{
		_blocks[(int)EBlockType::T][(int)EDirection::N].set(
			0, 1, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::T][(int)EDirection::W].set(
			0, 1, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::T][(int)EDirection::S].set(
			0, 0, 0, 0,
			1, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::T][(int)EDirection::E].set(
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);
	}
	
	// O자형 블록
	{
		_blocks[(int)EBlockType::O][(int)EDirection::N].set(
			1, 1, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::O][(int)EDirection::W].set(
			1, 1, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::O][(int)EDirection::S].set(
			1, 1, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::O][(int)EDirection::E].set(
			1, 1, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}

	// L자형 블록
	{
		_blocks[(int)EBlockType::L][(int)EDirection::N].set(
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::L][(int)EDirection::W].set(
			0, 0, 0, 0,
			0, 0, 1, 0,
			1, 1, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::L][(int)EDirection::S].set(
			0, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::L][(int)EDirection::E].set(
			1, 1, 1, 0,
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}
	
	// InvL자형 블록
	{
		_blocks[(int)EBlockType::InvL][(int)EDirection::N].set(
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 1, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::InvL][(int)EDirection::W].set(
			1, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::InvL][(int)EDirection::S].set(
			1, 1, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::InvL][(int)EDirection::E].set(
			0, 0, 0, 0,
			1, 0, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0
		);
	}

	// Z자형 블록
	{
		_blocks[(int)EBlockType::Z][(int)EDirection::N].set(
			0, 1, 0, 0,
			1, 1, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::Z][(int)EDirection::W].set(
			0, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::Z][(int)EDirection::S].set(
			0, 0, 1, 0,
			0, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::Z][(int)EDirection::E].set(
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}

	// InvZ자형 블록
	{
		_blocks[(int)EBlockType::InvZ][(int)EDirection::N].set(
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::InvZ][(int)EDirection::W].set(
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::InvZ][(int)EDirection::S].set(
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::InvZ][(int)EDirection::E].set(
			0, 0, 0, 0,
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0
		);
	}


	// 테스트용
	//_aaBoard[10][4] = 1;
	//_aaBoard[6][0] = 1;
	//_aaBoard[0][1] = 1;
	updateNextBlockQueue();

}

void fs::SimpleTetris::createBlock(EBlockType _eBlockType, const Color& color, uint8 alpha)
{
	_iiBlocks[(uint32)_eBlockType] = createBlankImage(kBlockSize);
	drawBlockUnitToImage(_eBlockType, Position2(0, 0), color, alpha);
}

void fs::SimpleTetris::drawBoard(const Position2& position, const Color& borderColor, const Color& boardColor)
{
	// 판 테두리
	drawRectangleToScreen(position - Position2(10, 10), kBoardSizePixel + Size2(20, 20), borderColor);

	// 판
	drawRectangleToScreen(position, kBoardSizePixel, boardColor);

	// 현재 블록
	drawBlockToBoard(_currBlockType, _currPos, _currdir, false);


	{
		const Position2 nextBlockOffset{ position + Position2(kBoardSizePixel.x + 50, 10) };

		// 다음 블록 테두리
		drawRectangleToScreen(nextBlockOffset, kBlockSize * Size2(4, 10), borderColor);

		// 다음 블록 판 
		drawRectangleToScreen(nextBlockOffset + kBlockSize * Size2(0.5f, 0.5f), kBlockSize * Size2(3, 9), boardColor);

		// 다음 블록들
		drawBlockToScreen(_nextBlockQueue[0], nextBlockOffset + kBlockSize * Size2(0.5f, 0.5f), EDirection::N);
		drawBlockToScreen(_nextBlockQueue[1], nextBlockOffset + kBlockSize * Size2(0.5f, 5.5f), EDirection::N);
	}


	for (float y = 0; y < kBoardSize.y; y += 1)
	{ // board 블록의 세로 
		for (float x = 0; x < kBoardSize.x; x += 1) // board 블록의 가로 
		{
			uint8 block = _aaBoard[(uint32)y][(uint32)x];

			if (block == (uint8)EBlockType::none)
			{
				__noop;
			}
			else
			{
				drawImageToScreen(_iiBlocks[block], position + kBlockSize * Position2(x, y));
			}
		}
	}
}

bool fs::SimpleTetris::move(EDirection _eDirection)
{
	drawBlockToBoard(_currBlockType, _currPos, _currdir, true);

	switch (_eDirection)
	{
	case fs::EDirection::N:
		if (canDrawBlock(_currBlockType, _currPos - Position2(0, 1), _currdir) == true)
		{
			_currPos.y -= 1;

			return true;
		}
		break;
	case fs::EDirection::E:
		// 다음 위치에 그릴 수 있으면 이동 시킨다
		if (canDrawBlock(_currBlockType, _currPos + Position2(1, 0), _currdir) == true)
		{
			_currPos.x += 1;
			return true;
		}
		break;
	case fs::EDirection::S:
		if (canDrawBlock(_currBlockType, _currPos + Position2(0, 1), _currdir) == true)
		{
			_currPos.y += 1;
			return true;
		}
		else
		{
			drawBlockToBoard(_currBlockType, _currPos, _currdir);
		
			if (_currPos.y < 0)
			{
				_isGameOver = true;
			}

			_currBlockType = _nextBlockQueue.front();
			_currdir = EDirection::N;
			_currPos.x = (kBoardSize.x / 2 - kBlockContainerSize / 2);
			_currPos.y = -(kBlockContainerSize / 2);

			_nextBlockQueue.pop_front();

			checkBingo();

		}
		break;
	case fs::EDirection::W:
		if (canDrawBlock(_currBlockType, _currPos - Position2(1, 0), _currdir) == true)
		{
			_currPos.x -= 1;
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

void fs::SimpleTetris::rotate()
{
	drawBlockToBoard(_currBlockType, _currPos, _currdir, true);

	int32 dir{ (int32)_currdir + 1 };
	if (dir == 4)
	{
		dir = 0;
	}
	EDirection nextDir = (EDirection)dir;
	if (canDrawBlock(_currBlockType, _currPos, nextDir) == true)
	{
		_currdir = nextDir;
	}
	else
	{
		Position2 resultPos{};
		if (getRotablePosition(nextDir, resultPos) == true)
		{
			if (canDrawBlock(_currBlockType, _currPos, nextDir) == true)
			{
				_currPos = resultPos;
				_currdir = nextDir;
			}
		}
	}
}

const fs::Position2& fs::SimpleTetris::getCurrPos() const
{
	return _currPos;
}

bool fs::SimpleTetris::getRotablePosition(EDirection _eNextdir, fs::Position2& outPosition) const
{
	outPosition = _currPos;
	bool shouldPush{ false };
	const auto& nextBlock{ _blocks[(uint32)_currBlockType][(uint32)_eNextdir] };
	int32 minX{};
	int32 maxX{};
	for (int32 y = 0; y < 4; ++y)
	{
		for (int32 x = 0; x < 4; ++x)
		{
			const uint8 blockValue{ nextBlock.data[y][x] };

			if (blockValue == 0) continue;

			const int32 nextX{ (int32)_currPos.x + x };
			if (nextX < 0)
			{
				minX = min(minX, nextX);
			}
			else if (nextX >= (int32)kBoardSize.x)
			{
				maxX = max(maxX, nextX);
			}
		}
	}

	if (minX != 0)
	{
		// minX가 음수이기 때문에 -야 커진다.
		// outPosition.x = 0;
		outPosition.x -= minX;
		shouldPush = true;
	}
	else if (maxX != 0)
	{
		outPosition.x -= (maxX - (int32)kBoardSize.x + 1);
		shouldPush = true;
	}
	return shouldPush;
}

fs::EBlockType fs::SimpleTetris::getCurrentBlockType() const
{
	return _currBlockType;
}

void fs::SimpleTetris::setCurrentBlockType(EBlockType eBlockType)
{
	drawBlockToBoard(_currBlockType, _currPos, _currdir, true);

	if (canDrawBlock(eBlockType, _currPos, _currdir) == true)
	{
		_currBlockType = eBlockType;
	}
	else
	{
		drawBlockToBoard(_currBlockType, _currPos, _currdir);
	}
}

fs::EBlockType fs::SimpleTetris::getNextBlockType() const
{
	int32 iBlocktype = (rand() % 7) + 2;	
	return EBlockType(iBlocktype);
}


void fs::SimpleTetris::drawBlockUnitToImage(EBlockType _eBlockType, const Position2& position, const Color& color, uint8 alpha)
{
	uint32 imageIndex{ _iiBlocks[(uint32)_eBlockType] };
	drawRectangleToImage(imageIndex, position, kBlockSize, color, alpha);

	drawRectangleToImage(imageIndex, position + Position2(kBlockSize.x, 0) - Position2(kBlockBorder, 0),
		Size2(kBlockBorder, kBlockSize.y), Color::sub(color, Color(60, 60, 60)), alpha);
	drawRectangleToImage(imageIndex, position + Position2(0, kBlockSize.y) - Position2(0, kBlockBorder),
		Size2(kBlockSize.x, kBlockBorder), Color::sub(color, Color(60, 60, 60)), alpha);

	drawRectangleToImage(imageIndex, position,
		Size2(kBlockSize.x - 1, kBlockBorder), Color::add(color, Color(60, 60, 60)), alpha);
	drawRectangleToImage(imageIndex, position,
		Size2(kBlockBorder, kBlockSize.y - 1), Color::add(color, Color(60, 60, 60)), alpha);
}

void fs::SimpleTetris::drawBlockToBoard(EBlockType _eBlockType, const Position2& position, EDirection _eDirection, bool bErase)
{
	const int32 x = (int32)position.x;
	const int32 y = (int32)position.y;
	const uint8 blockType{ (bErase == true) ? (uint8)0 : (uint8)(_eBlockType) };
	const auto& block{ _blocks[(uint32)_eBlockType][(uint32)_eDirection] };
	for (int32 y_ = 0; y_ < 4; ++y_)
	{
		for (int32 x_ = 0; x_ < 4; ++x_)
		{
			const int32 finalX{ x + x_ };
			const int32 finalY{ y + y_ };
			const uint8 blockValue{ block.data[y_][x_] };
			if (blockValue == 0) continue;
			if (finalY < 0 || finalY >= (int32)kBoardSize.y) continue;
			if (finalX < 0 || finalX >= (int32)kBoardSize.x) continue;
			_aaBoard[finalY][finalX] = blockType;
		}
	}
}

void fs::SimpleTetris::drawBlockToScreen(EBlockType _eBlockType, const Position2& position, EDirection _eDirect)
{
	uint32 iBlockType{ (uint32)_eBlockType };
	const auto& block{ _blocks[(uint32)_eBlockType][(uint32)_eDirect] };
	for (float y = 0; y < kBlockContainerSize; y += 1)
	{
		for (float x = 0; x < kBlockContainerSize; x += 1)
		{
			if (block.data[(uint32)y][(uint32)x] != 0)
			{
				drawImageToScreen(_iiBlocks[iBlockType], position + kBlockSize * Position2(x, y));
			}
		}
	}
}

bool fs::SimpleTetris::canDrawBlock(EBlockType _eBlockType, const Position2& position, EDirection _eDirection) const
{
	const int32 x = (int32)position.x;
	const int32 y = (int32)position.y;
	
	const auto& block{ _blocks[(uint32)_eBlockType][(uint32)_eDirection] };
	for (int32 y_ = 0; y_ < 4; ++y_)
	{
		for (int32 x_ = 0; x_ < 4; ++x_)
		{
			const int32 finalX{ x + x_ };
			const int32 finalY{ y + y_ };
			const uint8 blockValue{ block.data[y_][x_] };
			if (blockValue == 0) continue;
			if (finalX < 0 || finalX >= (int32)kBoardSize.x) return false;
			if (finalY < 0) continue;
			if (finalY >= (int32)kBoardSize.y) return false;

			if (_aaBoard[finalY][finalX] != 0)
			{
				return false;
			}
		}
	}
	return true;
}

void fs::SimpleTetris::setTimerInterval(int32 interval)
{
	if (interval <= kTimerIntervalMin)
	{
		interval = kTimerIntervalMin;
	}
	_timerInterval = interval;

}

fs::int32 fs::SimpleTetris::getTimerInterval() const
{
	return _timerInterval;
}

bool fs::SimpleTetris::tickTimer() const
{
	using namespace std::chrono;

	//현재 시간을 가져온다.
	auto elapsed = duration_cast<milliseconds>(steady_clock::now() - _prevTime);

	if (elapsed.count() >= _timerInterval)
	{
		_prevTime = steady_clock::now();

		return true;
	}

	return false;
}

void fs::SimpleTetris::updateNextBlockQueue()
{
	//보험
	while(_nextBlockQueue.size() < kNextBlockQueueMin) 
	{
		_nextBlockQueue.push_back(getNextBlockType());
	}
	
	if ((_nextBlockQueue.size() < kNextBlockQueueMax) && (tickSecond() == true))
	{
		_nextBlockQueue.push_back(getNextBlockType());
	}
}

fs::uint32 fs::SimpleTetris::getScore()
{
	return _score;
}

bool fs::SimpleTetris::isGameOver() const
{
	return _isGameOver;
}

void fs::SimpleTetris::checkBingo()
{
	int32 currentY{ (int32)kBoardSize.y - 1 };
	
	uint32 bingoCount{};

	while (currentY >= 0)
	{
		bool isBingo{ true }; // 기본 값이 false
		for (uint32 x = 0; x < (uint32)kBoardSize.x; ++x)
		{
			if (_aaBoard[currentY][x] == 0)
			{
				isBingo = false; // 서순
				break;
			}
		}
		if (isBingo == true)
		{
			//memset(_aaBoard[currentY], 0, (size_t)kBoardSize.x);

			for (int32 y = currentY - 1; y >= 0; --y) // 확신이 들지 않는다면 unsigned를 쓰면 안된다.
			{
				memcpy(_aaBoard[y + 1], _aaBoard[y], (size_t)kBoardSize.x);
			}
			++bingoCount;
		}
		else
		{
			--currentY;
		}
	}
	_score += (bingoCount * bingoCount) * 100;
}
