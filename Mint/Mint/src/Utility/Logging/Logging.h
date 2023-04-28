#ifndef _MINT_LOGGING_H_
#define _MINT_LOGGING_H_


#include "Common/Common.h"
#include "Common/Timer.h"


#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace mint
{
	class CLogging
	{
	public:
		enum Mode
		{
			Mode_Console = 0,
			Mode_File,
		};

		enum Verbosity
		{
			Verbosity_Debug = 0,
			Verbosity_Release,
			Verbosity_Distr
		};

	public:
		STATIC_GET(CLogging, g_CLogging);

		bool initialize();

		void terminate();

		void reset();

		template< typename... ARGS >
		void log_info(const std::shared_ptr< spdlog::logger >& logger, ARGS&&... args);

		template< typename... ARGS >
		void log_warn(const std::shared_ptr< spdlog::logger >& logger, ARGS&&... args);

		template< typename... ARGS >
		void log_error(const std::shared_ptr< spdlog::logger >& logger, ARGS&&... args);

		template< typename... ARGS >
		void log_critical(const std::shared_ptr< spdlog::logger >& logger, ARGS&&... args);

		std::shared_ptr< spdlog::logger >& get_logger();

		f32 get_application_running_time();

		void set_mode(Mode mode);

	private:
		CTimer m_timer;

		Mode m_mode;

		Verbosity m_verbosity;

		f32 m_applicationStartingTime;

		static std::shared_ptr< spdlog::logger > s_consoleLog;

		static std::shared_ptr< spdlog::logger > s_fileLog;
	};


	template< typename... ARGS >
	void mint::CLogging::log_critical(const std::shared_ptr< spdlog::logger >& logger, ARGS&&... args)
	{
		switch (m_verbosity)
		{
		case Verbosity_Debug:
		{
#if MINT_DEBUG
			logger->critical(args...);
#endif
			break;
		}
		case Verbosity_Release:
		{
#if MINT_RELEASE
			logger->critical(args...);
#endif
			break;
		}
		case Verbosity_Distr:
		{
#if MINT_DISTR
			logger->critical(args...);
#endif
			break;
		}
		}
	}


	template< typename... ARGS >
	void mint::CLogging::log_error(const std::shared_ptr< spdlog::logger >& logger, ARGS&&... args)
	{
		switch (m_verbosity)
		{
		case Verbosity_Debug:
		{
#if MINT_DEBUG
			logger->error(args...);
#endif
			break;
		}
		case Verbosity_Release:
		{
#if MINT_RELEASE
			logger->error(args...);
#endif
			break;
		}
		case Verbosity_Distr:
		{
#if MINT_DISTR
			logger->error(args...);
#endif
			break;
		}
		}
	}


	template< typename... ARGS >
	void mint::CLogging::log_warn(const std::shared_ptr< spdlog::logger >& logger, ARGS&&... args)
	{
		switch (m_verbosity)
		{
		case Verbosity_Debug:
		{
#if MINT_DEBUG
			logger->warn(args...);
#endif
			break;
		}
		case Verbosity_Release:
		{
#if MINT_RELEASE
			logger->warn(args...);
#endif
			break;
		}
		case Verbosity_Distr:
		{
#if MINT_DISTR
			logger->warn(args...);
#endif
			break;
		}
		}
	}


	template< typename... ARGS >
	void mint::CLogging::log_info(const std::shared_ptr< spdlog::logger >& logger, ARGS&&... args)
	{
		switch(m_verbosity)
		{
		case Verbosity_Debug:
		{
#if MINT_DEBUG
			logger->info(args...);
#endif
			break;
		}
		case Verbosity_Release:
		{
#if MINT_RELEASE
			logger->info(args...);
#endif
			break;
		}
		case Verbosity_Distr:
		{
#if MINT_DISTR
			logger->info(args...);
#endif
			break;
		}
		}
	}


}


#define MINT_APP_TIME mint::CLogging::Get().get_application_running_time()

#define MINT_LOG_INFO(...) mint::CLogging::Get().log_info(mint::CLogging::Get().get_logger(), __VA_ARGS__)

#define MINT_LOG_WARN(...) mint::CLogging::Get().log_warn(mint::CLogging::Get().get_logger(), __VA_ARGS__)

#define MINT_LOG_ERROR(...) mint::CLogging::Get().log_error(mint::CLogging::Get().get_logger(), __VA_ARGS__)

#define MINT_LOG_CRITICAL(...) mint::CLogging::Get().log_critical(mint::CLogging::Get().get_logger(), __VA_ARGS__)


#endif