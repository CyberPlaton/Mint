#include "FunctionProfiler.h"

namespace mint::profiler
{

	bool CFunctionProfiler::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}

	void CFunctionProfiler::terminate()
	{
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}

	void CFunctionProfiler::reset()
	{

	}

	void CFunctionProfiler::run_profiler_thread()
	{
		_run();
	}

	void CFunctionProfiler::stop_profiler_thread()
	{
		_set_is_running(false);

		_wait_for_termination();
	}

	bool CFunctionProfiler::is_running()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool v = m_running;

		);

		return v;
	}

	void CFunctionProfiler::_run()
	{
		std::thread thread(&CFunctionProfiler::_internal_run, this);

		thread.detach();
	}

	void CFunctionProfiler::_internal_run()
	{
		_set_is_running(true);

		while (is_running())
		{
			if (_should_update())
			{
				_internal_computation();
			}
			else
			{
				std::this_thread::yield();
			}
		}

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_internalLoop = false;

		);
	}

	void CFunctionProfiler::_internal_computation()
	{
		// Update statistical data.
		for (auto& pair : m_queue)
		{
			while (!pair.second.empty())
			{
				auto function = mint::algorithm::vector_get_last_element_as< SFunction* >(pair.second);

				if (lookup(pair.first, function->m_name))
				{
					_add_up_function_data(pair.first, function);
				}
				else
				{
					_create_function_entry(pair.first, function);
				}


				mint::algorithm::vector_erase_last(pair.second);

				delete function; function = nullptr;
			}
		}

		m_queue.clear();


		// Recompute refined statistical data.
		for (auto& pair : m_stats)
		{
			auto& vmeantime = m_dataSortedByMeantime[pair.first];
			auto& vcallcount = m_dataSortedByCallcount[pair.first];

			_sort_functions_by_meantime(pair.second, vmeantime);
			_sort_functions_by_callcount(pair.second, vcallcount);
		}
	}

	void CFunctionProfiler::_set_is_running(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_running = value;

		);
	}

	void CFunctionProfiler::_wait_for_termination()
	{
		while (true)
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				const bool running = m_internalLoop;

			);

			if (!running) return;

			std::this_thread::sleep_for(std::chrono::duration< s32 >::zero());
		}
	}


	void CFunctionProfiler::add_function_to_queue(const String& function_category, SFunction* function)
	{
		auto h = mint::algorithm::djb_hash(function_category);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			mint::algorithm::vector_push_back(m_queue[h], function);

		);
	}

	bool CFunctionProfiler::_should_update()
	{
		auto interval = _get_update_interval_in_seconds();

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			if (!m_timer.has_started())
			{
				m_timer.start_timer();
			}

			const auto v = m_timer.seconds_elapsed() > interval;

		);

		return v;
	}

	void CFunctionProfiler::set_update_interval_in_seconds(u32 seconds)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_updateInterval = seconds;

		);
	}

	mint::u32 CFunctionProfiler::_get_update_interval_in_seconds()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const auto v = m_updateInterval;

		);

		return v;
	}


	SFunctionProfilerUtility::SFunctionProfilerUtility(const String& function_category, const String& function_name)
	{
		m_function = new SFunction();
		m_function->m_threadId = std::this_thread::get_id();
		m_function->m_callcount = 1;
		m_function->m_accumulatedExecutionTime = 0.0f;
		m_function->m_name = function_name;
		m_category = function_category;
		m_timer.start_timer();
	}


	SFunctionProfilerUtility::~SFunctionProfilerUtility()
	{
		m_function->m_accumulatedExecutionTime = m_timer.milliseconds_elapsed();
		CFunctionProfiler::Get().add_function_to_queue(m_category, m_function);
	}


	void CFunctionProfiler::_sort_functions_by_meantime(Vector< SFunction >& rawfunctions, Vector< SFunction >& output)
	{
		output = rawfunctions;

		mint::algorithm::vector_sort_with_lambda(output, [&](SFunction& lh, SFunction& rh)
			{
				return lh.get_mean_execution_time() > rh.get_mean_execution_time();
			});
	}

	void CFunctionProfiler::_sort_functions_by_callcount(Vector< SFunction >& rawfunctions, Vector< SFunction >& output)
	{
		output = rawfunctions;

		mint::algorithm::vector_sort_with_lambda(output, [&](SFunction& lh, SFunction& rh)
			{
				return lh.get_callcount() > rh.get_callcount();
			});
	}

	void CFunctionProfiler::_add_up_function_data(u64 function_category, SFunction* function)
	{
		auto& data = m_stats[function_category];

		for (auto& func : data)
		{
			if (func.m_name == function->m_name)
			{
				func.m_callcount++;
				func.m_accumulatedExecutionTime += function->m_accumulatedExecutionTime;

				return;
			}
		}
	}

	void CFunctionProfiler::_create_function_entry(u64 function_category, SFunction* function)
	{
		auto& data = m_stats[function_category];

		data.emplace_back(function);
	}

	bool CFunctionProfiler::lookup(const String& function_category, const String& function_name)
	{
		auto h = mint::algorithm::djb_hash(function_category);

		return lookup(h, function_name);
	}

	bool CFunctionProfiler::lookup(u64 function_category, const String& function_name)
	{
		auto& vector = m_stats[function_category];

		for (auto& it : vector)
		{
			if (it.m_name == function_name) return true;
		}

		return false;
	}

	mint::profiler::SFunction CFunctionProfiler::get_stats_for_function_raw(const String& function_category, const String& function_name)
	{
		auto h = mint::algorithm::djb_hash(function_category);

		if (lookup(h, function_name))
		{
			for (auto& func : m_stats[h])
			{
				if (func.m_name == function_name)
				{
					return func;
				}
			}
		}

		return SFunction{};
	}

	mint::Vector< mint::profiler::SFunction > CFunctionProfiler::get_stats_for_category_raw(const String& function_category)
	{
		auto h = mint::algorithm::djb_hash(function_category);

		return m_stats[h];
	}

	mint::Vector< mint::profiler::SFunction > CFunctionProfiler::get_stats_for_category_sorted_by_meantime(const String& function_category)
	{
		auto h = mint::algorithm::djb_hash(function_category);

		return m_dataSortedByMeantime[h];
	}

	mint::Vector< mint::profiler::SFunction > CFunctionProfiler::get_stats_for_category_sorted_by_callcount(const String& function_category)
	{
		auto h = mint::algorithm::djb_hash(function_category);

		return m_dataSortedByCallcount[h];
	}

	
	SFunction::SFunction()
	{
	}

	SFunction::SFunction(SFunction* other)
	{
		m_threadId = other->m_threadId;
		m_callcount = 1;
		m_accumulatedExecutionTime = other->m_accumulatedExecutionTime;
		m_name = other->m_name.c_str();
	}

}