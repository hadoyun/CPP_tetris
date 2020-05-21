#include "SimpleTetris.h"
#include "CheapTimer.h"
//shift+f12 : 참조검색. 해당 함수 혹은 변수가 사용된 위치를 알려줌.
/*
1. 홀드 기능 만들기
2. clear bingo 소리가 묻힘
3. 
*/
static constexpr hady::int32 g_kWidth{ 600 };
static constexpr hady::int32 g_kHeight{ 1000 };
static hady::SimpleTetris g_simpleTetris{ g_kWidth, g_kHeight };

LRESULT WINAPI WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return g_simpleTetris.processWindowProc(hWnd, Msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	using namespace hady;

	g_simpleTetris.set(L"Simple Tetris", hInstance, WinProc);

	g_simpleTetris.addFont(L"Consolas", 20, false);
	g_simpleTetris.addFont(L"Jokerman", 46, false);
	g_simpleTetris.addFont(L"휴먼매직체", 20, true);
	g_simpleTetris.addFont(L"휴먼매직체", 45, true);
	g_simpleTetris.addFont(L"굴림체", 85, true);

	const Position2 boardPosition{ 10, 80 };
	Color clearColor{ 240, 240, 255 };
	const Color difaultColor{ 0, 0, 100 };
	Color levelColor{ difaultColor };

	//처음 시작해주기
	g_simpleTetris.restartGame();
	
	std::string AssetDir = "Asset/";
	g_simpleTetris.createAudioObjects(AssetDir);

	//레벨업 애니메이션 효과를 위한 timer클레스 선언
	CheapTimer levelUpTimer{};
	// milli 단위로 설정
	levelUpTimer.set(1'500, CheapTimer::EUnit::milli);

	//게임은 계속해서 업데이트가 되어야한다.
	while (g_simpleTetris.update() == true)
	{
		g_simpleTetris.updateNextblockQueue();

		g_simpleTetris.updateGameLevel();

		//gameover가 아닌 상황.
		if (g_simpleTetris.isGameOver() == false)
		{	// ????
			if (g_simpleTetris.tickInput() == true)
			{	//퍼즈가 트루라면 입력을 받지 않고 움직이지 않기
				if (g_simpleTetris.getPause() == true)
				{
					__noop;
				}
				else
				{
					//0x8001: key가 처음 눌렸을 때를 의미함.
					if (GetAsyncKeyState(VK_LEFT) == SHORT(0x8001))
					{
						g_simpleTetris.move(EDirection::E);
					}
					if (GetAsyncKeyState(VK_RIGHT) == SHORT(0x8001))
					{
						g_simpleTetris.move(EDirection::W);
					}
					if (GetAsyncKeyState('Z') == SHORT(0x8001))
					{
						//g_simpleTetris.move(EDirection::N);
						g_simpleTetris.rotate(true);
					}
					if (GetAsyncKeyState('X') == SHORT(0x8001))
					{
						//g_simpleTetris.move(EDirection::N);
						g_simpleTetris.rotate(false);
					}
					if (GetAsyncKeyState(VK_DOWN) == SHORT(0x8001))
					{
						g_simpleTetris.move(EDirection::S);
					}
					if (GetAsyncKeyState(VK_UP) == SHORT(0x8001))
					{
						g_simpleTetris.playFastSound();
						while (g_simpleTetris.move(EDirection::S) == true)
						{

						}
					}
					if (g_simpleTetris.tickGameSpeedTimer() == true)
					{
						g_simpleTetris.move(EDirection::S);
					}
					if (GetAsyncKeyState('H') == SHORT(0x8001))
					{
						g_simpleTetris.holdBlock();
					}
				}
				if (GetAsyncKeyState('P') == SHORT(0x8001))
				{	//토클
					g_simpleTetris.togglePause();
				}

#if defined DEBUG || _DEBUG
				if (GetAsyncKeyState('W') == SHORT(0x8001))
				{
					auto timerInterval{ g_simpleTetris.getTimerInterval() };

					g_simpleTetris.setTimerInterval(timerInterval - 50);
				}
				if (GetAsyncKeyState('Q') == SHORT(0x8001))
				{
					auto currBlockType = g_simpleTetris.getCurrBlockType();
					uint32 iNextBlockType{ (uint32)currBlockType + 1 };
					if (iNextBlockType >= (uint32)EBlockType::MAX)
					{
						iNextBlockType = 2;
					}
					g_simpleTetris.setCurrBlockType((EBlockType)iNextBlockType);
				}
#endif 
			}
		}
		
		g_simpleTetris.beginRendering(clearColor);
		{
			g_simpleTetris.drawBoard(boardPosition, Color(0, 60, 100), Color(200, 200, 200));

			g_simpleTetris.useFont(1);
			g_simpleTetris.drawTextToScreen(Position2(0, 0), Size2(SimpleTetris::kBoardSizePixel.x + 20, boardPosition.y - 10), L"TETRIS", Color(200, 100, 100),
				EHorzAlign::Center, EVertAlign::Center);

			g_simpleTetris.useFont(2);
			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 250, 05), L"FPS: "	  + std::to_wstring(g_simpleTetris.getFps()),		difaultColor);
			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 250, 25), L"SCORE: " + std::to_wstring(g_simpleTetris.getCurrScore()), difaultColor);
			if (g_simpleTetris.getGameLevelUp() == true)
			{
				if (levelUpTimer.tick() == false)
				{
					levelColor += Color(0.9f, 1.9f, 2.9f);
				}
				else
				{
					levelUpTimer.reset();

					g_simpleTetris.resetGameLevelUp();
					levelColor = difaultColor;
				}
			}
			else
			{
				levelUpTimer.reset();
			}
			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 250, 45), L"LEVEL: " + std::to_wstring(g_simpleTetris.getCurrLevel()), levelColor);
			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 250, 65), L"EXP: " + std::to_wstring(g_simpleTetris.getCurrLevelScore()), difaultColor);
			
			
			g_simpleTetris.useFont(3);
			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 250, 465), L"COMBO " 
				+ std::to_wstring(g_simpleTetris.getComboCount()), difaultColor);

			if (g_simpleTetris.isGameOver() == true)
			{
				g_simpleTetris.useFont(3);
				g_simpleTetris.drawTextToScreen(Position2(0, 0), Size2(g_kWidth, g_kHeight), L"GAME OVER"
					, difaultColor, EHorzAlign::Center, EVertAlign::Center);
				if (GetAsyncKeyState('R') == SHORT(0x8001))
				{
					g_simpleTetris.restartGame();
				}
			}

			if (g_simpleTetris.getPause() == true)
			{
				g_simpleTetris.useFont(4);
				g_simpleTetris.drawTextToScreen(Position2(0, 0), Size2(g_kWidth, g_kHeight), L"PAUSE"
					, difaultColor, EHorzAlign::Center, EVertAlign::Center);
			}
		}
		g_simpleTetris.endRendering();
	}
	return 0;
}
