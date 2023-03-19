#ifndef _MINT_TIMER_H_
#define _MINT_TIMER_H_


#include "Common.h"

#include <chrono>


namespace mint
{
	class CTimer
	{
	public:
		CTimer() = default;
		~CTimer() = default;

		static u64 get_time();

		static String get_local_date_time();

		bool has_started();

		void start_timer();

		u64 seconds_elapsed();

		f64 milliseconds_elapsed();

		f64 microseconds_elapsed();


	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
	};
}

#endif