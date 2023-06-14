#ifndef _MINT_ANIMATOR_STACK_H_
#define _MINT_ANIMATOR_STACK_H_


#include "../Common/IAnimator.h"
#include "Utility/STL/Map2.h"


namespace mint::animation
{
	class CAnimatorStack
	{
	public:

		bool initialize();

		void terminate();

		void on_update(f32 dt);


		bool set_current_state(const String& state_name);

		void add_animation_state(const String& state_name);

		void remove_animation_state(const String& state_name);

		bool lookup_animation_state(const String& state_name);

		CAnimator* try_push_animator(entt::entity entity, const String& state_name, const String& animator_name, void* animator_data,
			CAnimator::Animator_on_animation_update update_function,
			CAnimator::Animator_on_animation_enter enter_function,
			CAnimator::Animator_on_animation_exit exit_function,
			CAnimator::Animator_on_animator_initialize initialize_function,
			CAnimator::Animator_on_animator_terminate terminate_function);

		void remove_entity_animator(const String& state_name, const String& animator_name);


	private:
		u64 m_currentState = -1;
		u64 m_previousState = -1;
		bool m_stateChange = false;
		

		std::unordered_map< u64, CMap2< CAnimator > > m_animators;

	};

}


#endif