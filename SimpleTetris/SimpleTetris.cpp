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

	createBlock(EBlockType::used, Color(100, 100, 100)); // board 블록을 그리는 함수 
	createBlock(EBlockType::I, Color(255, 100, 100));
	createBlock(EBlockType::T, Color(100, 255, 100));

	
	{// I형 블록 채우기
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

	{ // T자형 블록 
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


	_aaBoard[10][4] = 1;
	_aaBoard[6][0] = 1;
	_aaBoard[0][1] = 1;
}

void fs::SimpleTetris::createBlock(EBlockType _eBlockType, const Color& color, uint8 alpha)
{
	_iiBlocks[(uint32)_eBlockType] = createBlankImage(kBlockSize);
	drawBlockUnitToImage(_eBlockType, Position2(0, 0), color, alpha);
}

void fs::SimpleTetris::drawBoard(const Position2& position, const Color& borderColor, const Color& boardColor)
{
	// 테두리
	drawRectangleToScreen(position - Position2(10, 10), kBoardSizePixel + Size2(20, 20), borderColor);

	// 판
	drawRectangleToScreen(position, kBoardSizePixel, boardColor);

	// 현재 블록
	drawToBlock(_curreBlockType, _currPos, _currdir, false);
	
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

void fs::SimpleTetris::move(EDirection _eDirection)
{
	drawToBlock(_curreBlockType, _currPos, _currdir, true);

	switch (_eDirection)
	{
	case fs::EDirection::N:
		if (canDrawBlock(_curreBlockType, _currPos - Position2(0, 1), _currdir) == true)
		{
			_currPos.y -= 1;
		}
		break;
	case fs::EDirection::E:
		// 다음 위치에 그릴 수 있으면 이동 시킨다
		if (canDrawBlock(_curreBlockType, _currPos + Position2(1, 0), _currdir) == true)
		{
			_currPos.x += 1;
		}
		break;
	case fs::EDirection::S:
		if (canDrawBlock(_curreBlockType, _currPos + Position2(0, 1), _currdir) == true)
		{
			_currPos.y += 1;
		}
		break;
	case fs::EDirection::W:
		if (canDrawBlock(_curreBlockType, _currPos - Position2(1, 0), _currdir) == true)
		{
			_currPos.x -= 1;
		}
		break;
	default:
		break;
	}

}

void fs::SimpleTetris::rotate()
{
	drawToBlock(_curreBlockType, _currPos, _currdir, true);

	int32 dir{ (int32)_currdir + 1 };
	if (dir == 4)
	{
		dir = 0;
	}
	EDirection nextDir = (EDirection)dir;
	if (canDrawBlock(_curreBlockType, _currPos, nextDir))
	{
		_currdir = nextDir;
	}
	else
	{
		Position2 resultPos{};
		if (getRotablePosition(nextDir, resultPos) == true)
		{
			if (canDrawBlock(_curreBlockType, _currPos, nextDir))
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
	const auto& nextBlock{ _blocks[(uint32)_curreBlockType][(uint32)_eNextdir] };
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

void fs::SimpleTetris::drawToBlock(EBlockType _eBlockType, const Position2& position, EDirection _eDirection, bool bErase)
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
			if (finalY < 0 || finalY >= (uint32)kBoardSize.y) continue;
			if (finalX < 0 || finalX >= (uint32)kBoardSize.x) continue;
			_aaBoard[finalY][finalX] = blockType;
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
			if (finalY >= (uint32)kBoardSize.y) return false;
			if (finalX < 0 || finalX >= (uint32)kBoardSize.x) return false;

			if (_aaBoard[finalY][finalX] != 0)
			{
				return false;
			}
		}
	}
	return true;
}
