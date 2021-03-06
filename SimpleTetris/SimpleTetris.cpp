﻿#include "SimpleTetris.h"
#include <thread>
#include <cassert>

#include <string.h>





hady::SimpleTetris::SimpleTetris(int32 width, int32 height) : IGraphicalWindow(width, height)
{
	_bingoTimer.set(150, CheapTimer::EUnit::milli);
}

hady::SimpleTetris::~SimpleTetris()
{

}

void hady::SimpleTetris::set(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc)
{
	{
		// create 내부 함수 
		createInternal(title, hInstance, windowProc);

		// 시드 제공
		srand(std::chrono::steady_clock::now().time_since_epoch().count());
		
		// 프로젝트 속성 - 디버깅 - 작업 디렉토리 설정 (상대 경로의 기준)
		// 상대경로의 기준을 Project를 기준으로 하고싶다면 $(ProjectDir)
		// 상대경로의 기준을 Solution를 기준으로 하고싶다면 $(SolutionDir)
		createBlock(EBlockType::Used,	Color(100, 100, 100));
		createBlock(EBlockType::I,		Color(060, 220, 255));
		createBlock(EBlockType::T,		Color(160, 100, 255));
		createBlock(EBlockType::O,		Color(255, 255, 000));
		createBlock(EBlockType::L,		Color(255, 127, 000));
		createBlock(EBlockType::InvL,	Color(000, 000, 220));
		createBlock(EBlockType::Z,		Color(255, 000, 000));
		createBlock(EBlockType::S,		Color(000, 255, 060));
		createBlock(EBlockType::Bingo,	Color(255, 255, 255));
	}
	//각 블록을 넣는다.
	{
		// I형 블록
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

		// S자형 블록
		{
			_blocks[(int)EBlockType::S][(int)EDirection::N].set(
				0, 1, 0, 0,
				0, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 0, 0
			);

			_blocks[(int)EBlockType::S][(int)EDirection::W].set(
				0, 1, 1, 0,
				1, 1, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0
			);

			_blocks[(int)EBlockType::S][(int)EDirection::S].set(
				1, 0, 0, 0,
				1, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0
			);

			_blocks[(int)EBlockType::S][(int)EDirection::E].set(
				0, 0, 0, 0,
				0, 1, 1, 0,
				1, 1, 0, 0,
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
	}
	updateNextblockQueue();
}

void hady::SimpleTetris::drawGuideBlock(Position2 boardOffset)
{
	setBlockToBoard(_currBlockType, _currPosition, _currDirection, true);
	bool shouldDraw{ false };
	//max = 둘 중 큰 숫자를 가져오는 매크로임. 따라서 최소값을 지정하려면 max를 사용해야 함.(더 작은 숫자를 무시함.)
	for (int i = max(_currPosition.y, 0); i < kBoardSize.y; ++i)
	{
		if (canDrawBlock(_currBlockType, Position2(_currPosition.x, i), _currDirection) == true)
		{
			shouldDraw = true;
		}
		else
		{
			if (shouldDraw == true)
			{
				uint32 iCurrBlockType{ (uint32)_currBlockType };
				uint32 iCurrDirection{ (uint32)_currDirection };
				for (int32 y = 0; y < kBlockContainerSize; ++y)
				{
					for (int32 x = 0; x < kBlockContainerSize; ++x)
					{
						if (_blocks[iCurrBlockType][iCurrDirection].data[y][x] == 0) continue;

						drawImageAlphaToScreen(_iiBlocks[iCurrBlockType],
							boardOffset + kBlockSize * Position2(_currPosition.x + x, i - 1 + y), (uint8)127);
					}
				}
			}
			break;
		}
	}
	//drawBlockToBoard(_currBlockType, _currPosition, _currDirection, false);
}

void hady::SimpleTetris::drawBoard(const Position2& boardOffset, const Color& borderColor, const Color& boardColor)
{
	// 판 테두리
	drawRectangleToScreen(boardOffset - Position2(10, 10), kBoardSizePixel + Size2(20, 20), borderColor);

	// 판
	drawRectangleToScreen(boardOffset, kBoardSizePixel, boardColor);

	// 현재 블록
	setBlockToBoard(_currBlockType, _currPosition, _currDirection);

	// 격자무늬 그리드 그리기 
	drawGrid(boardOffset);

	Position2 nextBLockPosition{ boardOffset + Position2(kBoardSizePixel.x + 30, 10) };

	//다음 블록 테두리
	drawRectangleToScreen(nextBLockPosition, kBlockSize * Size2(5, 11), borderColor);

	//다음 블록들
	drawBlockToScreen(_nextBlockQueue[0], nextBLockPosition + kBlockSize, EDirection::N);
	drawBlockToScreen(_nextBlockQueue[1], nextBLockPosition + Size2(kBlockSize.x, kBlockSize.y * 6), EDirection::N);

	// hold block position
	Position2 holdBLockPosition{ boardOffset + Position2(kBoardSizePixel.x + 30, 430) };
	// 홀드 블록 테두리
	drawRectangleToScreen(holdBLockPosition, kBlockSize * Size2(5, 6), borderColor);
	// 홀드 블록 스크린
	if (_holdBlock.size() <= 0)
	{
		drawBlockToScreen(EBlockType::Used, holdBLockPosition + kBlockSize, EDirection::N);
	}
	else if(_holdBlock.size() >= 1)
	{
		drawBlockToScreen(_holdBlock[0], holdBLockPosition + kBlockSize, EDirection::N);
	}

	for (float y = 0; y < kBoardSize.y; y += 1)
	{
		for (float x = 0; x < kBoardSize.x; x += 1)
		{
			uint8 block{ _board[uint32(y)][uint32(x)] };

			if (block != 0)
			{
				drawImageToScreen(_iiBlocks[block], boardOffset + kBlockSize * Position2(x, y));
			}
		}
	}

	//가이드 블록 그리기
	drawGuideBlock(boardOffset);
}

void hady::SimpleTetris::drawGrid(const hady::Position2& startPosition)
{	// 가로 축을 그린다.
	for (int y = 0; y < (int)kBoardSize.y; ++y)
	{
		drawLineToScreen(Position2(startPosition.x, startPosition.y + (y * kBlockSize.y))
			, Position2((startPosition.x + kBoardSizePixel.x - 1), startPosition.y + (y * kBlockSize.y)), Color(250, 250, 250));
	}
	// 세로 축을 그린다.
	for (int x = 0; x < (int)kBoardSize.x; ++x)
	{
		drawLineToScreen(Position2(startPosition.x + (x * kBlockSize.x), startPosition.y)
			, Position2(startPosition.x + (x * kBlockSize.x), startPosition.y + (kBoardSizePixel.y - 1)), Color(250, 250, 250));
	}
}

bool hady::SimpleTetris::move(EDirection eDirection)
{
	//현재 블록을 지운다. bErase = true
	setBlockToBoard(_currBlockType, _currPosition, _currDirection, true);

	switch (eDirection)
	{
	case hady::EDirection::N:
		if (canDrawBlock(_currBlockType, _currPosition - Position2(0, 1), _currDirection) == true)
		{	
			_audio.playSound("이동");
			//이제 북쪽으론 움직이지 않기 때문에 필요 없다.
			//_currPosition.y -= 1;
			return true;
		}
		break;
	case hady::EDirection::W:
		if (canDrawBlock(_currBlockType, _currPosition + Position2(1, 0), _currDirection) == true)
		{	// 오른쪽으로 이동
			
			_currPosition.x += 1;
			_audio.playSound("이동");
			return true;
		}
		break;
	case hady::EDirection::S:
		if (canDrawBlock(_currBlockType, _currPosition + Position2(0, 1), _currDirection) == true)
		{	//아래쪽으로 이동
			_currPosition.y += 1;

			return true;
		}
		else
		{
			// 지워진 
			setBlockToBoard(_currBlockType, _currPosition, _currDirection);

			if (_currPosition == getInitialBlockPosition()) 
			{
				_isGameOver = true;
			}

			// 새 블록 스폰
			spawnNewBlock();

			if (canDrawBlock(_currBlockType, _currPosition, _currDirection) == false)
			{
				_isGameOver = true;
			}

			_nextBlockQueue.pop_front();

			checkBingo();
		}
		break;
	case hady::EDirection::E:
		if (canDrawBlock(_currBlockType, _currPosition - Position2(1, 0), _currDirection) == true)
		{
			_currPosition.x -= 1;
			_audio.playSound("이동");
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
 
void hady::SimpleTetris::rotate(bool clockWise)
{	//블록 지우기
	setBlockToBoard(_currBlockType, _currPosition, _currDirection, true);

	_audio.playSound("이동");

	int32 currDirection{ int32(_currDirection) };

	if (clockWise == true)
	{
		++currDirection;
		if (currDirection == 4)
		{
			currDirection = 0;
		}
	}
	else
	{
		--currDirection;
		if (currDirection == -1)
		{
			currDirection = 3;
		}
	}

	EDirection nextDirection = EDirection(currDirection);
	if (canDrawBlock(_currBlockType, _currPosition, nextDirection) == true)
	{
		_currDirection = nextDirection;
	}
	else
	{
		Position2 resultPosition{};
		if (getRotatablePosition(nextDirection, resultPosition) == true)
		{
			if (canDrawBlock(_currBlockType, resultPosition, nextDirection) == true)
			{
				_currPosition = resultPosition;
				_currDirection = nextDirection;
			}
		}
	}
}

const bool hady::SimpleTetris::getRotatablePosition(EDirection eNextDirection, hady::Position2& outPosition) const
{
	outPosition = _currPosition;
	bool shouldPush{ false };
	const auto& nextBlock{ _blocks[(uint32)_currBlockType][(uint32)eNextDirection] };
	int32 minX{};
	int32 maxX{};

	for (int32 y = 0; y < 4; ++y)
	{
		for (int32 x = 0; x < 4; ++x)
		{
			const uint8 blockValue{ nextBlock.data[y][x] };
			//회전시에 어떤 블록도 채워져 있지 않다면 계속해라
			if (blockValue == 0) continue;

			const int32 nextX{ (int32)_currPosition.x + x };
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
		//minX가 음수이고, outPosition이 음수인 상황이므로 -를 해줘야 함.
		//outPosition.x -= minX;
		outPosition.x = 0;
		shouldPush = true;
	}
	else if (maxX != 0)
	{
		outPosition.x -= (maxX - (int32)kBoardSize.x + 1);
		shouldPush = true;
	}
	return shouldPush;
}

const hady::Position2& hady::SimpleTetris::getCurrPosition() const
{
	return _currPosition;
}
// 사악한 디버깅용 함수
void hady::SimpleTetris::setCurrBlockType(EBlockType eBlockType)
{
	setBlockToBoard(_currBlockType, _currPosition, _currDirection, true);

	if (canDrawBlock(eBlockType, _currPosition, _currDirection) == true)
	{
		_currBlockType = eBlockType;
	}
	else
	{
		setBlockToBoard(_currBlockType, _currPosition, _currDirection);
	}
}

hady::EBlockType hady::SimpleTetris::getCurrBlockType() const
{
	return _currBlockType;
}

hady::EBlockType hady::SimpleTetris::getRandomBlockType() const
{
	int32 iBlockType{ getRandomBlockTypeInternal() };
	while (_prevRandomBlockType == iBlockType)
	{
		iBlockType = getRandomBlockTypeInternal();
	}
	_prevRandomBlockType = iBlockType;

	return (EBlockType)iBlockType;
}

hady::int32 hady::SimpleTetris::getRandomBlockTypeInternal() const
{
	return (int32)(((double)rand() / (double)(RAND_MAX + 1)) * 7.0) + 2;
}

void hady::SimpleTetris::updateNextblockQueue()
{
	while (_nextBlockQueue.size() < kNextBlockQueueMinSize)
	{
		_nextBlockQueue.push_back(getRandomBlockType());
		// 여유주기 
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if (_nextBlockQueue.size() < kNextBlockQueueMaxSize && tickSecond() == true)
	{
		_nextBlockQueue.push_back(getRandomBlockType());
	}
}

void hady::SimpleTetris::setTimerInterval(int32 interval)
{
	if (interval <= kTimerIntervalMin)
	{
		interval = kTimerIntervalMin;
	}

	_gameSpeed = interval;
}

hady::int32 hady::SimpleTetris::getTimerInterval() const
{
	return _gameSpeed;
}

bool hady::SimpleTetris::tickGameSpeedTimer() const
{
	using namespace std::chrono;

	auto elapsed{ duration_cast<milliseconds>(steady_clock::now() - _prevTime) };

	if (elapsed.count() >= _gameSpeed)
	{
		_prevTime = steady_clock::now();

		return true;
	}

	return false;
}

void hady::SimpleTetris::updateGameLevel()
{
	if (_currExe >= _ExeForLevelUp && _currLevel < 100)
	{
		++_currLevel;
		_currExe = 0;
		_ExeForLevelUp += 500;

		if (_gameSpeed > 50)
		{
			_gameSpeed -= 40;
		}
		_isLevelup = true;
	}
}

hady::uint32 hady::SimpleTetris::getCurrScore() const
{
	return _currScore;
}

hady::uint32 hady::SimpleTetris::getCurrLevel() const
{
	return _currLevel;
}

hady::uint32 hady::SimpleTetris::getCurrLevelScore() const
{
	return _currExe;
}

bool hady::SimpleTetris::isGameOver() const
{
	return _isGameOver;
}

void hady::SimpleTetris::restartGame()
{
	_isGameOver = false;

	spawnNewBlock();

	_currLevel = 1;
	_currScore = 0;
	_currExe = 0;

	//memset으로 하면 됨.
	/*for (int32 y = 0; y < (int32)kBoardSize.y; ++y)
	{
		for (int32 x = 0; x < (int32)kBoardSize.x; ++x)
		{
			_board[y][x] = 0;
		}
	}*/

	//float * float은 연산 오버플로우가 발생하므로 앞의 하나를 double로 캐스팅 한다.
	// 보드를 비운다.
	memset(_board, 0, (size_t)((double)kBoardSize.x * kBoardSize.y));

	// 디버그용 블록
#if defined DEBUG || _DEBUG
	for (int y = kBoardSize.y - 1; y > kBoardSize.y - 4; --y)
	{
		for (int x = 0; x < kBoardSize.x; ++x)
		{
			_board[y][x] = 1;
		}
		_board[y][3] = 0;
	}
#endif 

	_nextBlockQueue.pop_front();
	_nextBlockQueue.pop_front();
	_nextBlockQueue.pop_front();

	_holdBlock.clear();

	_currScore = 0;
	_comboCount = 0;
}

void hady::SimpleTetris::spawnNewBlock()
{
	_currBlockType = _nextBlockQueue.front();
	switch (_currBlockType)
	{
	case hady::EBlockType::I:
		_currDirection = EDirection::E;
		break;
	case hady::EBlockType::T:
		_currDirection = EDirection::N;
		break;
	case hady::EBlockType::O:
		_currDirection = EDirection::N;
		break;
	case hady::EBlockType::L:
		_currDirection = EDirection::E;
		break;
	case hady::EBlockType::InvL:
		_currDirection = EDirection::W;
		break;
	case hady::EBlockType::Z:
		_currDirection = EDirection::E;
		break;
	case hady::EBlockType::S:
		_currDirection = EDirection::W;
		break;
	case hady::EBlockType::None:
	case hady::EBlockType::Used:
	case hady::EBlockType::Bingo:
	case hady::EBlockType::MAX:
		break;
	default:
		break;
	}
	_currPosition = getInitialBlockPosition();
	// 새 블록이 스폰 된 후에만 홀드가 가능하다.
	_canHold = true;
}

// 새로운 시작 블록 포지션
hady::Position2 hady::SimpleTetris::getInitialBlockPosition() const
{
	Position2 result{ (kBoardSize.x * 0.5) - (kBlockContainerSize * 0.5), 0 };

	return result;
}

//빙고 체크하기 
void hady::SimpleTetris::checkBingo()
{
	int32 bingoY{ (int32)kBoardSize.y - 1 };
	
	while (bingoY >= 0)
	{
		bool isBingo{ true };

		for (uint32 x = 0; x < (uint32)kBoardSize.x; ++x)
		{
			if (_board[bingoY][x] == 0)
			{
				isBingo = false;

				break;
			}
		}

		if (isBingo == true)
		{	
			_bingoLines.emplace_back(bingoY);

			changeBingoLineColor(bingoY);

			_bingoTimer.start();
		}
		--bingoY;
	}

	uint32 deltaScore{ (uint32)_bingoLines.size() * (uint32)_bingoLines.size() * 100 };

	_currScore += deltaScore;
	_currExe += deltaScore;
	_comboCount = 0;
}

void hady::SimpleTetris::changeBingoLineColor(int32 bingoedY)
{
	//빙고 체크는 화면에 나오는 곳 까지
	assert(bingoedY >= 0);
	assert(bingoedY < (int32)kBoardSize.y);

	//빙고가 된 행을 EBlockType::Bingo(흰색)로 대입한다.
	for (int32 x = 0; x < (int32)kBoardSize.x; ++x)
	{
		_board[bingoedY][x] = (int32)EBlockType::Bingo;
	}
}

void hady::SimpleTetris::clearBingoLine(int32 bingoedY)
{
	_audio.playSound("빙고");

	for (int32 y = bingoedY; y > -3; --y)
	{
		memcpy(_board[y], _board[y - 1], (size_t)kBoardSize.x);
	}
}

void hady::SimpleTetris::createBlock(EBlockType eBlockType, const Color& color, uint8 alpha)
{
	_iiBlocks[(uint32)eBlockType] = createBlankImage(kBlockSize);
	drawBlockUnitToImage(eBlockType, Position2(0, 0), color, alpha);
}

void hady::SimpleTetris::drawBlockUnitToImage(EBlockType eBlockType, const Position2& position, const Color& color, uint8 alpha)
{
	const auto imageIndex{ _iiBlocks[(uint32)eBlockType] };
	drawRectangleToImage(imageIndex, position, kBlockSize, color, alpha);

	//border: 테두리
	drawRectangleToImage(imageIndex, position + Position2(kBlockSize.x, 0) - Position2(kBlockBorder, 0),
		Size2(kBlockBorder, kBlockSize.y), Color::sub(color, Color(60, 60, 60)), alpha);
	drawRectangleToImage(imageIndex, position + Position2(0, kBlockSize.y) - Position2(0, kBlockBorder),
		Size2(kBlockSize.x, kBlockBorder), Color::sub(color, Color(60, 60, 60)), alpha);

	drawRectangleToImage(imageIndex, position,
		Size2(kBlockSize.x - 1, kBlockBorder), Color::add(color, Color(60, 60, 60)), alpha);
	drawRectangleToImage(imageIndex, position,
		Size2(kBlockBorder, kBlockSize.y - 1), Color::add(color, Color(60, 60, 60)), alpha);
}

void hady::SimpleTetris::setBlockToBoard(EBlockType eBlockType, const Position2& position, EDirection eDirection, bool bErase)
{
	const int32 x{ int32(position.x) };
	const int32 y{ int32(position.y) };
	// 삼항 연산자, bErase가 true면 None 번호(0)을 받는다. 아니라면 eBlockType의 번호(2~8)를 받는다
	const uint8 blockType{ (bErase == true) ? uint8(EBlockType::None) : uint8(eBlockType) };
	const auto& block = _blocks[(uint32)eBlockType][(uint32)eDirection];
	// 해당 블록이 들어갈 수 잇는지 없는지 계산
	for (int32 y_ = 0; y_ < 4; ++y_)
	{
		for (int32 x_ = 0; x_ < 4; ++x_)
		{
			const int32 finalX{ x + x_ };
			const int32 finalY{ y + y_ };
			const uint8 blockValue{ block.data[y_][x_] };
			if (blockValue == 0) continue;
			if (finalY <= -kBlockContainerSize || finalY >= (int32)kBoardSize.y) continue;
			if (finalX < 0 || finalX >= (int32)kBoardSize.x) continue;

			_board[finalY][finalX] = blockType;
		}
	}
}

void hady::SimpleTetris::drawBlockToScreen(EBlockType eBlockType, const Position2& position, EDirection eDirection)
{
	int32 blockType{ (int32)eBlockType };
	const auto& block{ _blocks[blockType][(uint32)eDirection] };
	// 왜 += 1 일까?  === float 이기 때문!
	for (float y = 0; y < kBlockContainerSize; y += 1)
	{
		for (float x = 0; x < kBlockContainerSize; x += 1)
		{
			if (block.data[uint32(y)][uint32(x)] != 0)
			{
				drawImageToScreen(_iiBlocks[blockType], position + kBlockSize * Position2(x, y));
			}
		}
	}
}

bool hady::SimpleTetris::canDrawBlock(EBlockType eBlockType, const Position2& position, EDirection eDirection) const
{
	const int32 x{ int32(position.x) };
	const int32 y{ int32(position.y) };
	const auto& block = _blocks[(uint32)eBlockType][(uint32)eDirection];
	
	for (int32 y_ = 0; y_ < 4; ++y_)
	{
		for (int32 x_ = 0; x_ < 4; ++x_)
		{
			const int32 finalX{ x + x_ };
			const int32 finalY{ y + y_ };
			const uint8 blockValue{ block.data[y_][x_] };
			// 블록이 들어갈 칸이 모두 비어있다면 블록은 들어갈 수 있다.
			if (blockValue == 0) continue; 
			// (x축) 보드 밖으로 나가면 안됀다.
			if (finalX < 0 || finalX >= (int32)kBoardSize.x)
			{
				return false;
			}
			// ????
			if (finalY < 0) continue;
			// (y축을 기준으로) 보드 밖으로 나가면 안됀다.
			if (finalY >= (int32)kBoardSize.y)
			{
				return false;
			}
			// ????
			if (_board[finalY][finalX] != 0)
			{
				return false;
			}
		}
	}
	return true;
}

void hady::SimpleTetris::togglePause() const
{
	_pause = !_pause;
}

bool hady::SimpleTetris::getPause() const
{
	return _pause;
}

hady::uint32 hady::SimpleTetris::getComboCount() const
{
	return _comboCount;
}

void hady::SimpleTetris::addComboCount()
{
	++_comboCount;
}

bool hady::SimpleTetris::getGameLevelUp() const
{
	return _isLevelup;
}

void hady::SimpleTetris::resetGameLevelUp()
{
	_isLevelup = false;
}

bool hady::SimpleTetris::update()
{
	if (_bingoTimer.tick() == true)
	{
		//의미 상 비교 크게 3부분 
		while (_bingoLines.size() > 0)
		{
			clearBingoLine(_bingoLines.back());
			_bingoLines.pop_back();
		}
		_bingoTimer.stop();
	}

	return __super::update();
}

void hady::SimpleTetris::createAudioObjects(const std::string& AssetDir)
{
	// 생성자를 호출하면 초기화도 해줘야지! 
	_audio.createSound(AssetDir + "Princess Maker 2 BGM Credit.mp3",				FMOD_LOOP_NORMAL,					 "배경음");
	_audio.createSound(AssetDir + "Jump-SoundBible.com-1007297584.mp3",				FMOD_DEFAULT | FMOD_NONBLOCKING,	 "빙고");
	_audio.createSound(AssetDir + "ppong.mp3",										FMOD_DEFAULT | FMOD_NONBLOCKING,	 "이동");
	_audio.createSound(AssetDir + "move1.mp3",										FMOD_DEFAULT | FMOD_NONBLOCKING,	 "빠른이동");
	_audio.createSound(AssetDir + "ppong.mp3",										FMOD_DEFAULT | FMOD_NONBLOCKING,	 "홀드");
	_audio.createSound(AssetDir + "ItemUpgrade_NoChange_1.mp3",						FMOD_DEFAULT | FMOD_NONBLOCKING,	 "실패");
	
	_audio.createSound(AssetDir + "big_explosion.wav",								FMOD_DEFAULT | FMOD_NONBLOCKING,	 "디버깅 효과음");
	_audio.createSound(AssetDir + "Kiryu Kyosuke Harmonica Theme Full.wav",			FMOD_DEFAULT | FMOD_NONBLOCKING,	 "배경음2");

	//배경음 플레이
	_audio.playSound("배경음");
}

void hady::SimpleTetris::stopSound(const std::string& soundKind)
{
	_audio.stopSound(soundKind);
}

void hady::SimpleTetris::holdBlock()
{
	if (_canHold == true)
	{
		//처음 홀드 할때
		if (_holdBlock.size() == 0)
		{
			if (canDrawBlock(_currBlockType, _currPosition, _currDirection) == true)
			{
				_audio.playSound("홀드");

				_holdBlock.emplace_back(_currBlockType);

				_currBlockType = _nextBlockQueue.front();

				_nextBlockQueue.pop_front();

				_prevHoldBlock = _currBlockType;

				_canHold = false;
			}
			else
			{
				__noop;
			}
		}
		// 두번째 부터 홀드를 할때
		else if (_prevHoldBlock != _currBlockType)
		{
			if (canDrawBlock(_holdBlock.front(), _currPosition, _currDirection) == true)
			{
				_audio.playSound("홀드");

				_holdBlock.emplace_back(_currBlockType);

				_currBlockType = _holdBlock.front();

				_holdBlock.pop_front();

				_prevHoldBlock = _holdBlock.front();

				_canHold = false;
			}
		}
		else
		{
			_audio.playSound("디버깅 효과음");
		}
		
	}
	else
	{	// 홀드가 실패 했을 시 나오는 효과음
		_audio.playSound("실패");
	}
}