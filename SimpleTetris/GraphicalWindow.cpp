﻿#include "GraphicalWindow.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cassert>
#include <wingdi.h>

#pragma comment(lib, "Msimg32.lib")


fs::IGraphicalWindow::IGraphicalWindow(int32 width, int32 height) : kWidth{ width }, kHeight{ height }
{
	__noop;
}

fs::IGraphicalWindow::~IGraphicalWindow()
{
	uninitialize();
}

void fs::IGraphicalWindow::createInternal(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc)
{
	// 윈도우 클래스를 등록한다
	WNDCLASSEXW windowClass{};
	windowClass.cbClsExtra = 0;
	windowClass.cbSize = sizeof(windowClass);
	windowClass.cbWndExtra = 0;
	windowClass.hbrBackground = nullptr; // 배경을 그리지 않게 한다
	windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	windowClass.hIconSm = windowClass.hIcon = LoadIconW(nullptr, IDI_SHIELD);
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = windowProc;
	windowClass.lpszClassName = title.c_str();
	windowClass.lpszMenuName = nullptr;
	windowClass.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClassExW(&windowClass);

	// 윈도우를 생성한다
	RECT windowRect{ 0, 0, 800, 600 };
	_hWnd = CreateWindowExW(0, windowClass.lpszClassName, windowClass.lpszClassName, WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, kWidth, kHeight,
		nullptr, nullptr, hInstance, nullptr);

	// 윈도우를 보여준다.
	ShowWindow(_hWnd, SW_SHOWDEFAULT);

	// 필요한 변수들을 초기화한다.
	initialize();
}

LRESULT fs::IGraphicalWindow::processWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void fs::IGraphicalWindow::addFont(const std::wstring& fontName, int32 size, bool isKorean)
{
	_vFonts.emplace_back(CreateFont(size, 0, 0, 0, 0, FALSE, FALSE, FALSE, (isKorean == true) ? HANGEUL_CHARSET : 0, 0, 0, 0, 0, fontName.c_str()));
}

void fs::IGraphicalWindow::useFont(uint32 fontIndex)
{
	assert(fontIndex < static_cast<uint32>(_vFonts.size()));
	SelectObject(_backDc, _vFonts[fontIndex]);
}

fs::uint32 fs::IGraphicalWindow::createImageFromFile(const std::wstring& fileName)
{
	char fileNameA[MAX_PATH]{};
	WideCharToMultiByte(CP_ACP, 0, fileName.c_str(), static_cast<int>(fileName.size()), fileNameA, MAX_PATH, 0, FALSE);

	int x{}, y{}, channelCount{};
	auto pixels = stbi_load(fileNameA, &x, &y, &channelCount, 4);

	HBITMAP bitmap{ CreateBitmap(x, y, 1, channelCount * 8, pixels) };
	_vImages.emplace_back(bitmap, Size2(static_cast<float>(x), static_cast<float>(y)));

	stbi_image_free(pixels);

	return static_cast<uint32>(_vImages.size() - 1);
}

fs::uint32 fs::IGraphicalWindow::createBlankImage(const Size2& size)
{
	HBITMAP bitmap{ CreateCompatibleBitmap(_backDc, static_cast<int>(size.x), static_cast<int>(size.y)) };
	_vImages.emplace_back(bitmap, size);

	return static_cast<uint32>(_vImages.size() - 1);
}

bool fs::IGraphicalWindow::update()
{
	MSG msg{};
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	++_frameCounter;

	timePoint currTime{ steadyClock::now() };

	// FPS
	{
		auto elapsed{ std::chrono::duration_cast<miliSec>(currTime - _prevFrameTime) };
		if (elapsed.count() >= 1000)
		{
			_fps = _frameCounter;
			_fpsWstring = std::to_wstring(_fps);
			_frameCounter = 0;
			_prevFrameTime = currTime;
		}
	}

	// Input Tick
	{
		auto elapsed{ std::chrono::duration_cast<miliSec>(currTime - _prevInputTime) };
		if (elapsed.count() >= 10)
		{
			_bInputTick = true;
			_prevInputTime = currTime;
		}
	}

	// Second Tick
	{
		auto elapsed{ std::chrono::duration_cast<miliSec>(currTime - _prevSecondTime) };
		if (elapsed.count() >= 1000)
		{
			_bSecondTick = true;
			_prevSecondTime = currTime;
		}
	}

	return true;
}

void fs::IGraphicalWindow::beginRendering(const Color& clearColor)
{
	// 윈도우의 상하좌우를 얻어온다.
	RECT windowRect{};
	GetClientRect(_hWnd, &windowRect);

	// _backDc를 clearColor로 클리어
	HBRUSH brush{ CreateSolidBrush(RGB(clearColor.r, clearColor.g, clearColor.b)) };
	FillRect(_backDc, &windowRect, brush);
	DeleteObject(brush);
}

void fs::IGraphicalWindow::endRendering()
{
	// _backDc를 _frontDc로 복사 , 더블 버퍼링 ... !
	BitBlt(_frontDc, 0, 0, kWidth, kHeight, _backDc, 0, 0, SRCCOPY);

	// 윈도우를 다시 그리도록 명령
	UpdateWindow(_hWnd);
}

void fs::IGraphicalWindow::drawRectangleToScreen(const Position2& position, const Size2& size, const Color& color, uint8 alpha)
{
	const LONG width{ static_cast<LONG>(size.x) };
	const LONG height{ static_cast<LONG>(size.y) };
	const HBRUSH brush{ CreateSolidBrush(RGB(color.r, color.g, color.b)) };

	if (alpha == 255)
	{
		RECT rect{};
		rect.left = static_cast<LONG>(position.x);
		rect.right = static_cast<LONG>(rect.left + width);
		rect.top = static_cast<LONG>(position.y);
		rect.bottom = static_cast<LONG>(rect.top + height);
		FillRect(_backDc, &rect, brush);
	}
	else
	{
		const HBITMAP bitmap{ CreateCompatibleBitmap(_backDc, width, height) };
		SelectObject(_tempDc, bitmap);

		RECT rect{};
		rect.left = 0;
		rect.right = width;
		rect.top = 0;
		rect.bottom = height;
		FillRect(_tempDc, &rect, brush);

		BLENDFUNCTION blend{};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.AlphaFormat = 0;
		blend.SourceConstantAlpha = alpha;
		AlphaBlend(_backDc, static_cast<int>(position.x), static_cast<int>(position.y),
			static_cast<int>(size.x), static_cast<int>(size.y),
			_tempDc, 0, 0, rect.right, rect.bottom, blend);

		DeleteObject(bitmap);
	}
	DeleteObject(brush);
}

void fs::IGraphicalWindow::drawRectangleToImage(uint32 imageIndex, const Position2& position, const Size2& size, const Color& color, uint8 alpha)
{
	assert(imageIndex < static_cast<uint32>(_vImages.size()));

	const LONG width{ static_cast<LONG>(size.x) };
	const LONG height{ static_cast<LONG>(size.y) };
	const HBRUSH brush{ CreateSolidBrush(RGB(color.r, color.g, color.b)) };

	SelectObject(_tempDc, _vImages[imageIndex].bitmap);

	if (alpha == 255)
	{
		RECT rect{};
		rect.left = static_cast<LONG>(position.x);
		rect.right = static_cast<LONG>(rect.left + width);
		rect.top = static_cast<LONG>(position.y);
		rect.bottom = static_cast<LONG>(rect.top + height);
		FillRect(_tempDc, &rect, brush);
	}
	else
	{
		const HDC emptyDc{ CreateCompatibleDC(_backDc) };
		const HBITMAP bitmap{ CreateCompatibleBitmap(_backDc, width, height) };
		SelectObject(emptyDc, bitmap);

		RECT rect{};
		rect.left = 0;
		rect.right = width;
		rect.top = 0;
		rect.bottom = height;
		FillRect(emptyDc, &rect, brush);

		BLENDFUNCTION blend{};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.AlphaFormat = 0;
		blend.SourceConstantAlpha = alpha;
		AlphaBlend(_tempDc, static_cast<int>(position.x), static_cast<int>(position.y),
			static_cast<int>(size.x), static_cast<int>(size.y),
			emptyDc, 0, 0, rect.right, rect.bottom, blend);

		DeleteObject(bitmap);
		DeleteDC(emptyDc);
	}
	DeleteObject(brush);
}

void fs::IGraphicalWindow::drawImageToScreen(uint32 imageIndex, const Position2& position)
{
	assert(imageIndex < static_cast<uint32>(_vImages.size()));
	const auto& image{ _vImages[imageIndex] };
	SelectObject(_tempDc, image.bitmap);

	BitBlt(_backDc, static_cast<int>(position.x), static_cast<int>(position.y),
		static_cast<int>(image.size.x), static_cast<int>(image.size.y),
		_tempDc, 0, 0, SRCCOPY);
}

void fs::IGraphicalWindow::drawImageAlphaToScreen(uint32 imageIndex, const Position2& position)
{
	assert(imageIndex < static_cast<uint32>(_vImages.size()));
	const auto& image{ _vImages[imageIndex] };
	SelectObject(_tempDc, image.bitmap);

	TransparentBlt(_backDc, static_cast<int>(position.x), static_cast<int>(position.y),
		static_cast<int>(image.size.x), static_cast<int>(image.size.y),
		_tempDc, 0, 0, static_cast<int>(image.size.x), static_cast<int>(image.size.y), 0);
}

void fs::IGraphicalWindow::drawImagePrecomputedAlphaToScreen(uint32 imageIndex, const Position2& position)
{
	assert(imageIndex < static_cast<uint32>(_vImages.size()));
	const auto& image{ _vImages[imageIndex] };
	SelectObject(_tempDc, image.bitmap);
	
	BLENDFUNCTION blend{};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.SourceConstantAlpha = 255;
	AlphaBlend(_backDc, static_cast<int>(position.x), static_cast<int>(position.y),
		static_cast<int>(image.size.x), static_cast<int>(image.size.y),
		_tempDc, 0, 0, static_cast<int>(image.size.x), static_cast<int>(image.size.y), blend);
}

void fs::IGraphicalWindow::drawTextToScreen(const Position2& position, const std::wstring& content, const Color& color)
{
	RECT rect{};
	rect.left = static_cast<LONG>(position.x);
	rect.top = static_cast<LONG>(position.y);
	SetTextColor(_backDc, RGB(color.r, color.g, color.b));
	DrawTextW(_backDc, content.c_str(), static_cast<int>(content.size()), &rect, DT_LEFT | DT_TOP | DT_NOCLIP | DT_SINGLELINE);
}

void fs::IGraphicalWindow::drawTextToScreen(const Position2& position, const Size2& area, const std::wstring& content, const Color& color,
	EHorzAlign eHorzAlign, EVertAlign eVertAlign)
{
	UINT HorzAlign{ static_cast<UINT>((eHorzAlign == EHorzAlign::Left) ? DT_LEFT : (eHorzAlign == EHorzAlign::Center) ? DT_CENTER : DT_RIGHT) };
	UINT VertAlign{ static_cast<UINT>((eVertAlign == EVertAlign::Top) ? DT_TOP : (eVertAlign == EVertAlign::Center) ? DT_VCENTER : DT_BOTTOM) };

	RECT rect{};
	rect.left = static_cast<LONG>(position.x);
	rect.top = static_cast<LONG>(position.y);
	rect.right = rect.left + static_cast<LONG>(area.x);
	rect.bottom = rect.top + static_cast<LONG>(area.y);
	SetTextColor(_backDc, RGB(color.r, color.g, color.b));
	DrawTextW(_backDc, content.c_str(), static_cast<int>(content.size()), &rect, HorzAlign | VertAlign | DT_NOCLIP | DT_SINGLELINE);
}

fs::uint32 fs::IGraphicalWindow::getFps() const noexcept
{
	return _fps;
}

const std::wstring& fs::IGraphicalWindow::getFpsWstring() const noexcept
{
	return _fpsWstring;
}

fs::int32 fs::IGraphicalWindow::getWidth() const noexcept
{
	return kWidth;
}

fs::int32 fs::IGraphicalWindow::getHeight() const noexcept
{
	return kHeight;
}

bool fs::IGraphicalWindow::tickInput() const noexcept
{
	if (_bInputTick == true)
	{
		_bInputTick = false;
		return true;
	}
	return false;
}

bool fs::IGraphicalWindow::tickSecond() const noexcept
{
	if (_bSecondTick == true)
	{
		_bSecondTick = false;
		return true;
	}
	return false;
}

void fs::IGraphicalWindow::initialize()
{
	// 현재 윈도우의 기본 Device Context를 얻어온다.
	_frontDc = GetDC(_hWnd);

	// 기본 폰트를 얻어올까?
	//_vFonts.emplace_back(GetStockObject(DEVICE_DEFAULT_FONT));

	// _frontDc와 동일한 설정의 DC를 만든다. (더블 버퍼링 용도)
	_backDc = CreateCompatibleDC(_frontDc);

	// _frontDc와 동일한 설정의 DC를 만든다. (임시 저장 용도)
	_tempDc = CreateCompatibleDC(_backDc);

	// _backDc와 _tempDc의 기본 배경을 투명하게 한다. 기본값은 OPAQUE(불투명)이다.
	SetBkMode(_backDc, TRANSPARENT);
	SetBkMode(_tempDc, TRANSPARENT);

	// _backDc에서 사용할 비트맵을 생성하고, 설정한다.
	_backDcBitmap = CreateCompatibleBitmap(_frontDc, kWidth, kHeight);
	SelectObject(_backDc, _backDcBitmap);
}

void fs::IGraphicalWindow::uninitialize()
{
	for (auto& image : _vImages)
	{
		DeleteObject(image.bitmap);
	}
	for (auto& font : _vFonts)
	{
		DeleteObject(font);
	}
	DeleteObject(_backDcBitmap);

	// CreateCompatibleDC() <> DeleteDC()
	DeleteDC(_tempDc);				
	DeleteDC(_backDc);			

	// GetDC() <> ReleaseDC()
	ReleaseDC(_hWnd, _frontDc);
}
