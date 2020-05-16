#pragma once

#include <Windows.h>
#include <cstdint>
#include <chrono>
#include <string>
#include <vector>
#include "Common.h"

namespace hady
{
	using steadyClock = std::chrono::steady_clock;
	using timePoint = steadyClock::time_point;
	using miliSec = std::chrono::milliseconds;

	struct Float2
	{
		constexpr Float2()
		{
			__noop;
		}
		constexpr Float2(float x_, float y_) : x{ x_ }, y{ y_ }
		{
			__noop;
		}
		float x{};
		float y{};

		constexpr Float2 operator+() const
		{
			return Float2(+x, +y);
		}
		constexpr Float2 operator-() const
		{
			return Float2(-x, -y);
		}

		constexpr Float2 operator+(const Float2& b) const
		{
			return Float2(x + b.x, y + b.y);
		}
		constexpr Float2 operator-(const Float2& b) const
		{
			return Float2(x - b.x, y - b.y);
		}
		constexpr Float2 operator*(const Float2& b) const
		{
			return Float2(x * b.x, y * b.y);
		}
	};
	using Position2 = Float2;
	using Size2 = Float2;


	struct Color
	{
		constexpr Color()
		{
			__noop;
		}
		constexpr Color(float r_, float g_, float b_) : r{ r_ }, g{ g_ }, b{ b_ }
		{
			__noop;
		}
		float r{};
		float g{};
		float b{};

		constexpr void add(const Color& o)
		{
			r = min(r + o.r, 255);
			g = min(g + o.g, 255);
			b = min(b + o.b, 255);
		}
		constexpr void sub(const Color& o)
		{
			r = max(r - o.r, 0);
			g = max(g - o.g, 0);
			b = max(b - o.b, 0);
		}

		static constexpr Color add(const Color& a, const Color& o)
		{
			return Color(min(a.r + o.r, 255), min(a.g + o.g, 255), min(a.b + o.b, 255));
		}
		static constexpr Color sub(const Color& a, const Color& o)
		{
			return Color(max(a.r - o.r, 0), max(a.g - o.g, 0), max(a.b - o.b, 0));
		}

		constexpr Color operator+(const Color& o) const
		{
			return Color(r + o.r, g + o.g, b + o.b);
		}
		constexpr Color operator-(const Color& o) const
		{
			return Color(r - o.r, g - o.g, b - o.b);
		}

		constexpr Color& operator+=(const Color& o)
		{
			r += o.r;
			g += o.g;
			b += o.b;
			return *this;
		}
		constexpr Color& operator-=(const Color& o)
		{
			r -= o.r;
			g -= o.g;
			b -= o.b;
			return *this;
		}
	};


	struct Image
	{
		Image()
		{
			__noop;
		}
		Image(HBITMAP bitmap_, const Size2& size_) : bitmap{ bitmap_ }, size{ size_ }
		{
			__noop;
		}
		HBITMAP	bitmap{};
		Size2	size{};
	};


	enum class EHorzAlign
	{
		Left,
		Center,
		Right
	};

	enum class EVertAlign
	{
		Top,
		Center,
		Bottom,
	};


	class IGraphicalWindow
	{
	public:
		IGraphicalWindow(int32 width, int32 height);
		virtual ~IGraphicalWindow();

	public:
		virtual void set(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc) abstract;

	protected:
		// create() 안에서 이 함수를 반드시 가장 먼저 호출하세요.
		void createInternal(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc);

	public:
		LRESULT processWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	public:
		void addFont(const std::wstring& fontName, int32 size, bool isKorean);
		void useFont(uint32 fontIndex);

		uint32 createImageFromFile(const std::wstring& fileName);
		uint32 createBlankImage(const Size2& size);

	public:
		virtual bool update();

	public:
		void beginRendering(const Color& clearColor);
		void endRendering();

	public:
		// 이 함수를 직접 호출하기보단, createBlankImage()와 drawRectangleToImage()를 이용하면 훨씬 성능에 좋습니다.
		void drawRectangleToScreen(const Position2& position, const Size2& size, const Color& color, uint8 alpha = 255);

		// createBlankImage(), drawImage...()와 함께 사용하면 drawRectangleToScreen()보다 더 좋은 성능을 낼 수 있는 함수입니다.
		void drawRectangleToImage(uint32 imageIndex, const Position2& position, const Size2& size, const Color& color, uint8 alpha = 255);

		void drawImageToScreen(uint32 imageIndex, const Position2& position);
		void drawImageAlphaToScreen(uint32 imageIndex, const Position2& position);
		void drawImageAlphaToScreen(uint32 imageIndex, const Position2& position, uint8 alpha);
		void drawImagePrecomputedAlphaToScreen(uint32 imageIndex, const Position2& position);
		void drawTextToScreen(const Position2& position, const std::wstring& content, const Color& color);
		void drawTextToScreen(const Position2& position, const Size2& area, const std::wstring& content, const Color& color,
			EHorzAlign eHorzAlign, EVertAlign eVertAlign);
		void drawLineToScreen(const Position2& positionA, const Position2& positionB, const Color& color);

		void gideBocks();

	public:
		uint32 getFps() const noexcept;
		const std::wstring& getFpsWstring() const noexcept;
		int32 getWidth() const noexcept;
		int32 getHeight() const noexcept;
		bool tickInput() const noexcept;
		bool tickSecond() const noexcept;



	private:
		void initialize();
		void uninitialize();

	protected:
		static constexpr uint32	kFpsBufferSize{ 20 };
		const int32				g_kWidth{ 800 };
		const int32				g_kHeight{ 600 };

	private:
		HWND					_hWnd{};
		HDC						_frontDc{};
		HDC						_backDc{};
		HBITMAP					_backDcBitmap{};
		HDC						_tempDc{};
		std::vector<HFONT>		_vFonts{};
		std::vector<Image>		_vImages{};

	private:
		timePoint				_prevFrameTime{};
		timePoint				_prevInputTime{};
		timePoint				_prevSecondTime{};
		uint32					_frameCounter{};
		uint32					_fps{};
		std::wstring			_fpsWstring{};
		mutable bool			_bInputTick{ false };
		mutable bool			_bSecondTick{ false };
	};
}
