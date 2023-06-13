#ifndef _MINTSCRIPTING_BEHAVIOR_ENGINE_H_
#define _MINTSCRIPTING_BEHAVIOR_ENGINE_H_


#include "Behavior/Behavior.h"
#include "../../IMint.h"
#include "Utility/STL/Map2.h"



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

		void stop_behavior_engine_thread();

		void set_all_behaviors_active(bool value);

		bool are_behaviors_active();

		bool is_running();

		void set_should_update(bool value);


		void create_behavior_script_prefab(const String& script_name, const String& script_file_path);

		bool set_behavior_for_entity(const String& script_name, entt::entity entity);

		void remove_behavior_from_entity(entt::entity entity);

		bool does_entity_have_behavior_set(entt::entity entity);

		CBehavior& get_entity_behavior(entt::entity entity);

		bool is_entity_behavior_active(entt::entity entity);

		const Vector< std::pair< String, String > >& get_all_behavior_prefabs();

	private:
		static CBehaviorEngine* s_CBehaviorEngine;

		bool m_internalLoop;

		bool m_running;

		bool m_update;

		bool m_behaviorsActive;

		MINT_CRITICAL_SECTION(m_criticalSection);


		CMap2< CBehavior > m_activeBehaviors;

		CMap< std::pair< String, String > > m_behaviorPrefabs;


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