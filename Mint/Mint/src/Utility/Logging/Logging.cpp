#include "Logging.h"

namespace mint
{
	std::shared_ptr< spdlog::logger > CLogging::s_consoleLog;
	std::shared_ptr< spdlog::logger > CLogging::s_fileLog;


	bool CLogging::initialize()
	{
		m_applicationStartingTime = 0.0f;
		
#if MINT_DEBUG
		m_verbosity = Verbosity_Debug;
		m_mode = Mode_Console;
		SetTraceLogLevel(LOG_WARNING | LOG_ERROR | LOG_FATAL);
#elif MINT_RELEASE
		m_verbosity = Verbosity_Release;
		m_mode = Mode_Console;
		SetTraceLogLevel(LOG_WARNING | LOG_ERROR | LOG_FATAL);
#else
		m_verbosity = Verbosity_Distr;
		m_mode = Mode_File;
		SetTraceLogLevel(LOG_NONE);
#endif
		

#if MINT_DISTR
		spdlog::set_pattern("%^[%n] %v%$");
		spdlog::flush_every(std::chrono::seconds(1));
		spdlog::flush_on(spdlog::level::level_enum::trace);

		s_fileLog = spdlog::basic_logger_mt("DISTR", "Mint.log");
#else
#if MINT_DEBUG
		spdlog::set_pattern("%^[%n] %v%$");
		s_consoleLog = spdlog::stdout_color_mt("DEBUG");
#elif MINT_RELEASE
		spdlog::set_pattern("%^[%n] %v%$");
		s_consoleLog = spdlog::stdout_color_mt("RELEASE");
#endif

#endif

		m_timer.start_timer();

		String date = CTimer::get_local_date_time();

		MINT_LOG_INFO("[{}] Application start...", date);

		return true;
	}


	void CLogging::terminate()
	{
		String date = CTimer::get_local_date_time();

		MINT_LOG_INFO("[{}] Application end...", date);

		s_fileLog.reset();
		s_consoleLog.reset();
	}


	void CLogging::reset()
	{
		MINT_LOG_INFO("Resetting logger...");
		
		m_applicationStartingTime = 0.0f;

		m_timer.start_timer();
	}


	std::shared_ptr< spdlog::logger >& CLogging::get_logger()
	{
		switch(m_mode)
		{
		case Mode_Console: return s_consoleLog;
		case Mode_File: return s_fileLog;
		default: return s_consoleLog;
		}
	}


	mint::f32 CLogging::get_application_running_time()
	{
		return m_timer.seconds_elapsed();
	}


	void CLogging::set_mode(Mode mode)
	{
		m_mode = mode;
	}


}