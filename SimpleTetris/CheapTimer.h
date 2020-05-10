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

	public:
		void set(uint32 interval, EUnit unit);
		
		void reset();
		
		void start();

		bool tick() const;


	private:
		uint32			_interval{};
		uint64			_startTime{};
		EUnit			_unit{ EUnit::second };
	};
}