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
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CBehaviorEngine::reset()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto & behaviors = m_activeBehaviors.get_all();

			while (!behaviors.empty())
			{
				behaviors[0].on_destroy();

				behaviors.erase(behaviors.begin());
			}

		);

		m_behaviorPrefabs.reset();
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

			for(auto& script : m_activeBehaviors.get_all())
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


	void CBehaviorEngine::set_behavior_for_entity(const String& script_name, entt::entity entity)
	{
		auto h = mint::algorithm::djb_hash(script_name);

		if(m_behaviorPrefabs.lookup(h))
		{

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				CBehavior& behavior = m_activeBehaviors.emplace_back(SCAST(u64, entity),
																	 m_behaviorPrefabs.get(h));

				behavior.initialize();

				behavior.set_script_entity(entity);

				behavior.on_create();

 			);

		}
	}


	void CBehaviorEngine::create_behavior_script_prefab(const String& script_name, const String& script_file_path)
	{
		auto h = mint::algorithm::djb_hash(script_name);

		CBehavior behavior;

		behavior.set_script_entity(entt::null);
		behavior.set_script_name(script_name);
		behavior.set_script_path(script_file_path);

		if (behavior.initialize())
		{
			m_behaviorPrefabs.add(h, behavior);
		}
	}


	void CBehaviorEngine::remove_behavior_from_entity(const String& script_name, entt::entity entity)
	{
		auto h = mint::algorithm::djb_hash(script_name);

		if (m_activeBehaviors.lookup(h))
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				auto& behavior = m_activeBehaviors.get_ref(h);

				behavior.on_destroy();

				m_activeBehaviors.remove(h);
			);
		}
	}


	void CBehaviorEngine::stop_behavior_engine_thread()
	{
		set_should_update(false);

		_set_is_running(false);

		_wait_for_termination();
	}


}