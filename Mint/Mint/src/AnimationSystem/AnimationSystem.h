#ifndef _MINT_ANIMATION_SYSTEM_H_
#define _MINT_ANIMATION_SYSTEM_H_


#include "AnimatorStack/AnimatorStack.h"
#include "../../IMint.h"

// Include animators for importing them from file.
#include "Animators/ColorAnimator.h"
#include "Animators/FrameAnimator.h"
#include "Animators/ScaleAnimator.h"
#include "Animators/TranslationAnimator.h"


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

		void set_all_animations_active(bool value);

		bool are_animations_active();


		bool request_entity_registration(entt::entity entity);
		
		bool is_entity_registered(entt::entity entity);


		void create_animation_prefab(const String& animation_name, const String& animation_file_path);

		bool set_entity_animation(entt::entity entity, const String& animation_name);




		void set_entity_animation_state(entt::entity entity, const String& state_name);

		void add_entity_animation_state(entt::entity entity, const String& state_name);
		
		void remove_entity_animation_state(entt::entity entity, const String& state_name);

		template< class T >
		CAnimator* try_push_entity_animator(entt::entity entity, const String& state_name, const String& animator_name, T* animator_data,
			CAnimator::Animator_on_animation_update update_function,
			CAnimator::Animator_on_animation_enter enter_function,
			CAnimator::Animator_on_animation_exit exit_function,
			CAnimator::Animator_on_animator_initialize initialize_function,
			CAnimator::Animator_on_animator_terminate terminate_function);
		
		void remove_entity_animator(entt::entity entity, const String& state_name, const String& animator_name);



	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		bool m_internalLoop;

		bool m_running;

		bool m_update;

		bool m_animationsActive;

		std::unordered_map< u64, CAnimatorStack > m_animatorStacks;

		CMap< std::pair< String, String > > m_animationPrefabs;

	private:
		void _run();

		void _internal_run();

		void _internal_computation();

		void _set_is_running(bool value);

		bool _should_update();

		void _wait_for_termination();
	};


	template< class T >
	CAnimator* mint::animation::CAnimationSystem::try_push_entity_animator(entt::entity entity, const String& state_name, const String& animator_name, T* animator_data,
		CAnimator::Animator_on_animation_update update_function,
		CAnimator::Animator_on_animation_enter enter_function,
		CAnimator::Animator_on_animation_exit exit_function,
		CAnimator::Animator_on_animator_initialize initialize_function,
		CAnimator::Animator_on_animator_terminate terminate_function)
	{
		mint::animation::CAnimator* animator = nullptr;

		if (is_entity_registered(entity))
		{
			auto h = SCAST(u64, entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				animator = m_animatorStacks[h].try_push_animator(entity, state_name, animator_name, (void*)animator_data, 
					update_function,
					enter_function,
					exit_function,
					initialize_function,
					terminate_function);

			);
		}

		return animator;
	}


	namespace frameanim
	{
		CAnimator* importer(entt::entity entity, const String& state_name, const String& animator_name, const String& animator_type, CAnimatorStack& stack, maml::SNode* node);
	}
	namespace coloranim
	{
		CAnimator* importer(entt::entity entity, const String& state_name, const String& animator_name, const String& animator_type, CAnimatorStack& stack, maml::SNode* node);
	}
	namespace scaleanim
	{
		CAnimator* importer(entt::entity entity, const String& state_name, const String& animator_name, const String& animator_type, CAnimatorStack& stack, maml::SNode* node);
	}
	namespace translationanim
	{
		CAnimator* importer(entt::entity entity, const String& state_name, const String& animator_name, const String& animator_type, CAnimatorStack& stack, maml::SNode* node);
	}
}

#endif