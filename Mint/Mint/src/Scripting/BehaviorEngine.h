#ifndef _MINTSCRIPTING_BEHAVIOR_ENGINE_H_
#define _MINTSCRIPTING_BEHAVIOR_ENGINE_H_


#include "Behavior/Behavior.h"
#include "../../IMint.h"


namespace mint::scripting
{

	class CBehaviorEngine
	{
	public:
		STATIC_GET(CBehaviorEngine, s_CBehaviorEngine);
	
		bool initialize();

		void terminate();

		void reset();

		void run_behavior_engine_thread();


		bool is_running();

		void set_should_update(bool value);


	private:
		static CBehaviorEngine* s_CBehaviorEngine;


		bool m_internalLoop;

		bool m_running;

		bool m_update;

		MINT_CRITICAL_SECTION(m_criticalSection);


		CMap< CBehavior > m_behaviors;


	private:
		void _run();

		void _internal_run();

		void _internal_computation();

		void _set_is_running(bool value);

		bool _should_update();

		void _wait_for_termination();
	};

}

#endif