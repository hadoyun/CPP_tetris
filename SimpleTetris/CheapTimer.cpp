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
	}

	void CheapTimer::reset()
	{
		start();
	}

	void CheapTimer::start()
	{
		_startTime = std::chrono::steady_clock::now().time_since_epoch().count();
	}

	bool CheapTimer::tick() const
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

		if (elapsed >= _interval) return true;
		return false;
	}
}