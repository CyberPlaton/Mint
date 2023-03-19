#include "Timer.h"


namespace mint
{


	u64 CTimer::get_time()
	{
		return std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}


	String CTimer::get_local_date_time()
	{
		time_t t;
		tm* tm = nullptr;

		time(&t);

		tm = localtime(&t);

		char date_string[100];

		strftime(date_string, 50, "%d. %B %Y %T", tm);

		std::string s(date_string);

		return String(s.c_str());
	}


	bool CTimer::has_started()
	{
		return m_startTimePoint.time_since_epoch().count() > 0;
	}


	void CTimer::start_timer()
	{
		m_startTimePoint = std::chrono::high_resolution_clock::now();
	}


	u64 CTimer::seconds_elapsed()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		f64 start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
		f64 end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		f64 duration = end - start;

		return SCAST(u64, duration / (1000.0 * 1000.0));
	}


	f64 CTimer::milliseconds_elapsed()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		f64 start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
		f64 end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		f64 duration = end - start;

		return duration / 1000.0;
	}


	f64 CTimer::microseconds_elapsed()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		f64 start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
		f64 end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		f64 duration = end - start;

		return duration;
	}


}