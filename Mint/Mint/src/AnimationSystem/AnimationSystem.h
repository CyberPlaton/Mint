#ifndef _MINT_ANIMATION_SYSTEM_H_
#define _MINT_ANIMATION_SYSTEM_H_


#include "AnimatorStack/AnimatorStack.h"
#include "../../IMint.h"


namespace mint::animation
{
	class CAnimationSystem
	{
	public:
		STATIC_GET(CAnimationSystem, s_CAnimationSystem);

		bool initialize();

		void terminate();

		void reset();

		void run_animation_system_thread();

		void stop_animation_system_thread();

		bool is_running();

		void set_should_update(bool value);





		bool request_entity_registration(entt::entity entity);
		
		bool is_entity_registered(entt::entity entity);


		void set_entity_animation_state(entt::entity entity, const String& state_name);

		void add_entity_animation_state(entt::entity entity, const String& state_name);
		
		void remove_entity_animation_state(entt::entity entity, const String& state_name);


		CAnimator* try_push_entity_animator(entt::entity entity, const String& state_name, const String& animator_name);
		
		void remove_entity_animator(entt::entity entity, const String& state_name, const String& animator_name);



	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		bool m_internalLoop;

		bool m_running;

		bool m_update;



		std::unordered_map< u64, CAnimatorStack > m_animatorStacks;

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