#ifndef _MINTSCRIPTING_SCRIPT_ENGINE_H_
#define _MINTSCRIPTING_SCRIPT_ENGINE_H_


#include "Script/Script.h"
#include "../../IMint.h"


namespace mint::scripting
{

	class CScriptEngine
	{
	public:
		STATIC_GET(CScriptEngine, s_CScriptEngine);

		bool initialize();

		void terminate();

		void reset();

		void run_script_engine_thread();


		bool is_running();

		void set_should_update(bool value);


	private:
		static CScriptEngine* s_CScriptEngine;


		bool m_internalLoop;

		bool m_running;

		bool m_update;

		MINT_CRITICAL_SECTION(m_criticalSection);


		CMap< CScript > m_scripts;


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