#include "Logging.h"

namespace mint
{
	std::shared_ptr< spdlog::logger > CLogging::s_consoleLog;


	std::shared_ptr< spdlog::logger > CLogging::s_fileLog;


	bool CLogging::initialize()
	{
		m_applicationStartingTime = 0.0f;
		m_verbosity = Verbosity_Debug;
		m_mode = Mode_Console;


#ifdef MINT_DEBUG
		spdlog::set_pattern("%^[%n] %v%$");
		s_consoleLog = spdlog::stdout_color_mt("DEBUG");
#else
		spdlog::set_pattern("%^[%n] %v%$");
		spdlog::flush_every(std::chrono::seconds(1));
		spdlog::flush_on(spdlog::level::level_enum::trace);

#ifdef MINT_RELEASE
		s_fileLog = spdlog::basic_logger_mt("RELEASE", "ApplicationLog.log");
#endif
#ifdef MINT_DISTR
		s_fileLog = spdlog::basic_logger_mt("DISTR", "ApplicationLog.log");
#endif
#endif
		m_timer.start_timer();

		MINT_LOG_INFO("Application start...");

		return true;
	}


	void CLogging::terminate()
	{
		MINT_LOG_INFO("Application end...");

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