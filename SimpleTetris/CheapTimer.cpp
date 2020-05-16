#include "CheapTimer.h"


//몇 초에 한번 지났는지 알려주는 아이

namespace hady
{
	CheapTimer::CheapTimer()
	{

	}

	CheapTimer::~CheapTimer()
	{

	}

	void CheapTimer::set(uint32 interval, EUnit unit)
	{
		_unit = unit;

		_interval = interval;

		start();
	}

	void CheapTimer::reset()
	{
		_startTime = std::chrono::steady_clock::now().time_since_epoch().count();

		_tickCount = 0;
	}

	void CheapTimer::start()
	{
		_isTicking = true;
		
		reset();
	}

	void CheapTimer::stop()
	{
		_isTicking = false;
	}

	void CheapTimer::update()
	{
		uint64 elapsed = std::chrono::steady_clock::now().time_since_epoch().count() - _startTime;

		switch (_unit)
		{
		case hady::CheapTimer::EUnit::second:
			elapsed /= 1'000'000'000;
			break;
		case hady::CheapTimer::EUnit::milli:
			elapsed /= 1'000'000;
			break;
		case hady::CheapTimer::EUnit::micro:
			elapsed /= 1'000;
			break;
		case hady::CheapTimer::EUnit::nano:
			//elapsed *= 1;
			__noop;
			break;
		default:
			break;
		}

		if (elapsed >= _interval)
		{
			++_tickCount;	
		}
	}

	bool CheapTimer::tick()
	{	
		//조건문이 명령문의 중간에 들어갈 수 있다. 
		//return (_tickCount > 0) ? true : false;
		update();

		if ((_isTicking == true) && (_tickCount > 0))
		{
			reset();
			return true;
		}
		else
		{
			return false;
		}
	}
}