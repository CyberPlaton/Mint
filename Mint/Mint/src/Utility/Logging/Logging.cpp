#include "Logging.h"

namespace mint
{

	mint::CTimer CLogging::s_timer;


	mint::CLogging::Mode CLogging::s_mode = Mode::Mode_Console;


	mint::CLogging::Verbosity CLogging::s_verbosity = Verbosity::Verbosity_Debug;


	mint::f32 CLogging::s_applicationStartingTime = 0.0f;


	std::shared_ptr< spdlog::logger > CLogging::s_consoleLog;


	std::shared_ptr< spdlog::logger > CLogging::s_fileLog;


	bool CLogging::initialize()
	{
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
		s_timer.start_timer();

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
		
		s_applicationStartingTime = 0.0f;

		s_timer.start_timer();
	}


	std::shared_ptr< spdlog::logger >& CLogging::get_logger()
	{
		switch(s_mode)
		{
		case Mode_Console: return s_consoleLog;
		case Mode_File: return s_fileLog;
		default: return s_consoleLog;
		}
	}


	mint::f32 CLogging::get_application_running_time()
	{
		return s_timer.get_time();
	}


}