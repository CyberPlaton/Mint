#include "BehaviorEngine.h"


namespace mint::scripting
{

	mint::scripting::CBehaviorEngine* CBehaviorEngine::s_CBehaviorEngine = nullptr;


	bool CBehaviorEngine::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		m_activeBehaviors.initialize(10000);

		m_internalLoop = false;
		m_running = false;
		m_update = false;
		m_behaviorsActive = true;

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
			
			auto behavior = m_activeBehaviors.begin();
			while (behavior)
			{

				if (m_behaviorsActive && behavior->is_ready()) behavior->on_destroy();


				behavior = m_activeBehaviors.advance(behavior);
			}

			m_activeBehaviors.reset();
			m_behaviorPrefabs.reset();

		);

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

			auto behavior = m_activeBehaviors.begin();
			u32 index = 0;
			while (behavior)
			{
				if (m_behaviorsActive && behavior->is_ready() && behavior->is_active()) behavior->on_update(dt);

				index++;
				behavior = m_activeBehaviors.advance(behavior);
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
			auto script_pair = m_behaviorPrefabs.get(h);

			if(does_entity_have_behavior_set(entity)) remove_behavior_from_entity(entity);

			auto behavior = m_activeBehaviors.emplace(SCAST(u64, entity));

			behavior->set_script_name(script_name);

			behavior->set_script_path(script_pair.second);

			behavior->set_script_entity_verified(entity_get_handle(entity));


			behavior->initialize();

			behavior->on_create();



			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,
 			);
		}
	}


	void CBehaviorEngine::create_behavior_script_prefab(const String& script_name, const String& script_file_path)
	{
		auto h = mint::algorithm::djb_hash(script_name);

		m_behaviorPrefabs.add(h, std::make_pair(script_name.c_str(), script_file_path.c_str()));
	}


	void CBehaviorEngine::remove_behavior_from_entity(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto behavior = m_activeBehaviors.get(h);

			behavior->on_destroy();

			m_activeBehaviors.remove(h);
 		);
	}


	void CBehaviorEngine::stop_behavior_engine_thread()
	{
		set_should_update(false);

		_set_is_running(false);

		_wait_for_termination();
	}


	bool CBehaviorEngine::does_entity_have_behavior_set(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto result = m_activeBehaviors.lookup(h);
			
		);

		return result;
	}


	mint::scripting::CBehavior& CBehaviorEngine::get_entity_behavior(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto behavior = m_activeBehaviors.get(h);

		);

		return *behavior;
	}


	bool CBehaviorEngine::is_entity_behavior_active(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto behavior = m_activeBehaviors.get(h);

			auto active = behavior->is_active();
		
		);

		return active;
	}


	const mint::Vector< std::pair< String, String > >& CBehaviorEngine::get_all_behavior_prefabs()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const auto & prefabs = m_behaviorPrefabs.get_all_const();

		);

		return prefabs;
	}


	void CBehaviorEngine::set_all_behaviors_active(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_behaviorsActive = value;

		);
	}


}