#ifndef _MINTSCRIPTING_SCRIPT_ENGINE_H_
#define _MINTSCRIPTING_SCRIPT_ENGINE_H_


#include "Script/Script.h"
#include "../../IMint.h"
#include "Utility/STL/Map2.h"


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

		void stop_script_engine_thread();


		bool is_running();

		void set_should_update(bool value);

		void add_script_to_scene(const String& script_name, const String& script_file_path);

		void remove_script_from_scene(const String& script_name);

		void set_all_scripts_active(bool value);

		bool are_scripts_active();

	private:
		static CScriptEngine* s_CScriptEngine;


		bool m_internalLoop;

		std::atomic_bool m_running;

		bool m_update;

		bool m_scriptsActive;

		MINT_CRITICAL_SECTION(m_criticalSection);


		CMap2< CScript > m_scripts;


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