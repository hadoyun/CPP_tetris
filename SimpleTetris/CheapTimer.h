#pragma once
#include "Common.h"
#include <chrono>

//몇 초에 한번 지났는지 알려주는 아이
namespace hady 
{
	class CheapTimer
	{
	public:
		enum class EUnit
		{
			second,
			milli,
			micro,
			nano,
		};

	public:
		CheapTimer();
		~CheapTimer();

	private:
		void update();

	public:
		void start();

		void stop();

		//호출한 시점의 시간으로 _starttime 시간으로 바꿔준다.
		void set(uint32 interval, EUnit unit);

		//_tickCount를 0으로 만들고, 다시 처음부터 시간을 잰다.
		//tick()을 호출할 경우, 이 함수를 호출하지 않아도 된다.
		void reset();

		//set에서 정한 시간이 흐르면 _tickCount가 증가한다.
		//_tickCount가 0보다 클때, reset()을 호출하고, true를 리턴한다.
		bool tick();


	private:
		uint32			_interval{};
		uint64			_startTime{};
		EUnit			_unit{ EUnit::second };
		uint32			_tickCount{};
		bool			_isTicking{ false };
	};
}