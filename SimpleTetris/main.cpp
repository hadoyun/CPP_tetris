﻿#include "SimpleTetris.h"

static constexpr fs::int32 kWidth{ 400 };
static constexpr fs::int32 kHeight{ 600 };
//생성자를 통해 전역변수로 SimpleTetris 생성, 
static fs::SimpleTetris g_simpleTetris{ kWidth, kHeight };

//윈도우 프록시 생성
LRESULT WINAPI WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return g_simpleTetris.processWindowProc(hWnd, Msg, wParam, lParam);
}

//윈도우 메인 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	using namespace fs;

	g_simpleTetris.create(L"Simple Tetris", hInstance, WinProc);

	g_simpleTetris.addFont(L"Consolas", 20, false);
	g_simpleTetris.addFont(L"Jokerman", 46, false);

	const Position2 boardPosition{ 10, 60 };
	Color clearColor{ 240, 240, 255 };
	Color fpsColor{ 0, 0, 100 };
	while (g_simpleTetris.update() == true)
	{
		if (g_simpleTetris.tickInput() == true)
		{
			if (GetAsyncKeyState(VK_LEFT) == (short)0x8001) // 0x8001 - 처음 한번 눌렀을 때만 확인함
			{
				g_simpleTetris.move(EDirection::W);
			}
			if (GetAsyncKeyState(VK_RIGHT) == (short)0x8001)
			{
				g_simpleTetris.move(EDirection::E);
			}
			if (GetAsyncKeyState(VK_UP) == (short)0x8001)
			{
				g_simpleTetris.move(EDirection::N);
			}
			if (GetAsyncKeyState(VK_DOWN) == (short)0x8001)
			{
				g_simpleTetris.move(EDirection::S);
			}
			if (GetAsyncKeyState(VK_SPACE) == (short)0x8001)
			{
				g_simpleTetris.rotate();
			}
		}

		g_simpleTetris.beginRendering(clearColor);
		{
			g_simpleTetris.drawBoard(boardPosition, Color(0, 60, 100), Color(200, 200, 200));
			
			//g_simpleTetris.drawImageAlphaToScreen(1, position); // L자 블록을 그리는 함수

			g_simpleTetris.useFont(1); // 폰트 사용
			g_simpleTetris.drawTextToScreen(Position2(0, 0), Size2(SimpleTetris::kBoardSizePixel.x + 20, boardPosition.y - 10), L"TETRIS", Color(200, 100, 100),
				EHorzAlign::Center, EVertAlign::Center); // 텍스트 그리기 

			g_simpleTetris.useFont(0); 
			g_simpleTetris.drawTextToScreen(Position2(kWidth - 100, 0), L"FPS: " + g_simpleTetris.getFpsWstring(), fpsColor);
			
			g_simpleTetris.drawTextToScreen(Position2(kWidth - 100, 20), L"CRP: " 
				+ std::to_wstring((int)g_simpleTetris.getCurrPos().x)
				+ L","
				+ std::to_wstring((int)g_simpleTetris.getCurrPos().y), Color(0,0,0));
		}
		g_simpleTetris.endRendering();
	}
	return 0;
}