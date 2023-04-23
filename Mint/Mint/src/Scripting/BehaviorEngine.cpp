#include "BehaviorEngine.h"


namespace mint::scripting
{

	mint::scripting::CBehaviorEngine* CBehaviorEngine::s_CBehaviorEngine = nullptr;


	bool CBehaviorEngine::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		m_internalLoop = false;
		m_running = false;
		m_update = false;

		return true;
	}


	void CBehaviorEngine::terminate()
	{
		_set_is_running(false);

		_wait_for_termination();
	}


	void CBehaviorEngine::reset()
	{
		m_behaviors.reset();
	}


	bool CBehaviorEngine::is_running()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool v = m_running;

		);

		return v;
	}


	void CBehaviorEngine::set_should_update(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_update = value;

		);
	}


	void CBehaviorEngine::_run()
	{
		std::thread thread(&CBehaviorEngine::_internal_run, this);

		thread.detach();
	}


	void CBehaviorEngine::_internal_run()
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


	void CBehaviorEngine::_internal_computation()
	{
		set_should_update(false);

		f32 dt = MINT_ENGINE()->get_engine_frametime();

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			for(auto& script : m_behaviors.get_all())
			{
				if (script.is_ready()) script.on_update(dt);
			}

		);
		

	}


	void CBehaviorEngine::_set_is_running(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_running = value;

		);
	}


	bool CBehaviorEngine::_should_update()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool v = m_update;

		);

		return v;
	}


	void CBehaviorEngine::_wait_for_termination()
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


	void CBehaviorEngine::run_behavior_engine_thread()
	{
		_run();
	}


}