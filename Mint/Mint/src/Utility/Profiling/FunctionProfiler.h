#ifndef _MINT_FUNCTION_PROFILER_H_
#define _MINT_FUNCTION_PROFILER_H_


#include "Common/Common.h"
#include "Common/Timer.h"
#include "Common/Algorithm.h"

#include <unordered_map>


namespace mint::profiler
{

	struct SFunction
	{
		SFunction();
		SFunction(SFunction* other);


		f32 get_mean_execution_time() { return m_callcount > 0 ? m_accumulatedExecutionTime / m_callcount : 0.0f; }

		u64 get_callcount() { return m_callcount; }


		std::thread::id m_threadId;

		String m_name;

		u64 m_callcount = 0;

		f32 m_accumulatedExecutionTime = 0.0f;
	};


	class CFunctionProfiler
	{
	public:
		STATIC_GET(CFunctionProfiler, s_CFunctionProfiler);

		bool initialize();

		void terminate();

		void reset();


		void run_profiler_thread();

		void stop_profiler_thread();

		bool is_running();

		void set_update_interval_in_seconds(u32 seconds);

		u32 get_update_interval_in_seconds();


		void add_function_to_queue(const String& function_category, SFunction* function);

		bool lookup(const String& function_category, const String& function_name);

		bool lookup(u64 function_category, const String& function_name);



		std::unordered_map< u64, String > get_all_categories();

		bool does_category_exist(const String& function_category);


		SFunction get_stats_for_function_raw(const String& function_category, const String& function_name);

		mint::Vector< SFunction > get_stats_for_category_raw(const String& function_category);

		mint::Vector< SFunction > get_stats_for_category_sorted_by_meantime(const String& function_category);

		mint::Vector< SFunction > get_stats_for_category_sorted_by_callcount(const String& function_category);


	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		bool m_internalLoop;

		bool m_running;

		mint::CTimer m_timer;

		u32 m_updateInterval = 0;

		// Do not use STL as we want to profile them too.
		std::unordered_map< u64, Vector< SFunction > > m_stats;

		std::unordered_map< u64, Vector< SFunction > > m_dataSortedByMeantime;

		std::unordered_map< u64, Vector< SFunction > > m_dataSortedByCallcount;

		std::unordered_map< u64, Vector< SFunction* > > m_queue;

		std::unordered_map< u64, String > m_categories;


	private:
		void _run();

		void _internal_run();

		void _internal_computation();

		void _set_is_running(bool value);

		bool _should_update();

		void _wait_for_termination();
		
		u32 _get_update_interval_in_seconds();

		void _restart_timer();

		void _sort_functions_by_meantime(Vector< SFunction >& rawfunctions, Vector< SFunction >& output);

		void _sort_functions_by_callcount(Vector< SFunction >& rawfunctions, Vector< SFunction >& output);

		void _add_up_function_data(u64 function_category, SFunction* function);

		void _create_function_entry(u64 function_category, SFunction* function);

	};




	struct SFunctionProfilerUtility
	{
		SFunctionProfilerUtility(const String& function_category, const String& function_name);
		~SFunctionProfilerUtility();


	protected:
		CTimer m_timer;
		SFunction* m_function;
		String m_category;

	protected:
		SFunctionProfilerUtility& operator=(const CFunctionProfiler& rh) = delete;
		SFunctionProfilerUtility(const CFunctionProfiler& rh) = delete;

	};


#if MINT_PROFILE
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define MINT_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define MINT_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define MINT_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define MINT_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define MINT_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define MINT_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define MINT_FUNC_SIG __func__
#else
#define MINT_FUNC_SIG "MINT_FUNC_SIG unknown"
#endif
#endif


#if MINT_DISTR
#define MINT_PROFILE_SCOPE_EX(name, category, variable_name)
#define	MINT_PROFILE_SCOPE_CAT(name, category)
#define	MINT_PROFILE_SCOPE(category)
#else
#define MINT_PROFILE_SCOPE_EX(name, category, variable_name) mint::profiler::SFunctionProfilerUtility variable_name(name, category)
#define MINT_PROFILE_SCOPE(name, category) mint::profiler::SFunctionProfilerUtility profiled_function(name, category)
#define MINT_PROFILE_FUNC(category) MINT_PROFILE_SCOPE(MINT_FUNC_SIG, category)
#endif

}

#endif