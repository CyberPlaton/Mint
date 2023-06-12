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


		void set_current_state(const String& state_name);

		void add_animation_state(const String& state_name);

		void remove_animation_state(const String& state_name);

		bool lookup_animation_state(const String& state_name);

		CAnimator* try_push_animator(entt::entity entity, const String& state_name, const String& animator_name);

		void remove_entity_animator(const String& state_name, const String& animator_name);


	private:
		String m_currentState;

		std::unordered_map< u64, CMap2< CAnimator > > m_animators;

	};

}


#endif