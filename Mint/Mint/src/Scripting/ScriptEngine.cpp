#include "ScriptEngine.h"


namespace mint::scripting
{

	mint::scripting::CScriptEngine* CScriptEngine::s_CScriptEngine = nullptr;


	bool CScriptEngine::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		m_scripts.initialize(10000);

		m_internalLoop = false;
		m_running = false;
		m_update = false;
		m_scriptsActive = true;

		return true;
	}


	void CScriptEngine::terminate()
	{
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CScriptEngine::reset()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto script = m_scripts.begin();
			while (script)
			{

				if (m_scriptsActive && script->is_ready()) script->on_destroy();


				script = m_scripts.advance(script);
			}

			m_scripts.reset();
		);
	}


	bool CScriptEngine::is_running()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool v = m_running;

 		);

		return v;
	}


	void CScriptEngine::set_should_update(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_update = value;

		);
	}


	void CScriptEngine::_run()
	{
		std::thread thread(&CScriptEngine::_internal_run, this);

		thread.detach();
	}


	void CScriptEngine::_internal_run()
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


	void CScriptEngine::_internal_computation()
	{
		set_should_update(false);

		f32 dt = MINT_ENGINE()->get_engine_frametime();

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto script = m_scripts.begin();
			u32 index = 0;
			while (script)
			{
				if (m_scriptsActive && script->is_ready() && script->is_active()) script->on_update(dt);

				index++;
				script = m_scripts.advance(script);
			}

		);
	}


	void CScriptEngine::_set_is_running(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_running = value;

 		);
	}


	bool CScriptEngine::_should_update()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool v = m_update;

		);

		return v;
	}


	void CScriptEngine::_wait_for_termination()
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


	void CScriptEngine::run_script_engine_thread()
	{
		_run();
	}


	void CScriptEngine::add_script_to_scene(const String& script_name, const String& script_file_path)
	{
		auto h = mint::algorithm::djb_hash(script_name);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto script = m_scripts.emplace(h);

			script->set_script_entity(entt::null);

			script->set_script_name(script_name);
			
			script->set_script_path(script_file_path);

			if (script->initialize())
			{
				script->on_create();
			}
			else
			{
				m_scripts.remove(h);
			}
		);
	}


	void CScriptEngine::remove_script_from_scene(const String& script_name)
	{
		auto h = mint::algorithm::djb_hash(script_name);

		if(m_scripts.lookup(h))
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				auto script = m_scripts.get(h);

				script->on_destroy();

				m_scripts.remove(h);

			);

		}
	}


	void CScriptEngine::stop_script_engine_thread()
	{
		set_should_update(false);

		_set_is_running(false);
		
		_wait_for_termination();
	}


	void CScriptEngine::set_all_scripts_active(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_scriptsActive = value;

		);
	}


}