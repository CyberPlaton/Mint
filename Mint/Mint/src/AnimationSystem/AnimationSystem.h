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

		template < class T, typename... ARGS >
		bool try_push_entity_animator(entt::entity entity, const String& animator_name, ARGS&&... args);

		template < class T, typename... ARGS >
		bool try_push_entity_animator_at_index(entt::entity entity, const String& animator_name, u64 index, ARGS&&... args);

		void remove_entity_animator(entt::entity entity, const String& animator_name);

		CAnimator* get_entity_animator(entt::entity entity, const String& animator_name);

		template < class T >
		T* get_entity_animator_as(entt::entity entity, const String& animator_name);


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

	template < class T >
	T* mint::animation::CAnimationSystem::get_entity_animator_as(entt::entity entity, const String& animator_name)
	{
		T* result = nullptr;

		if (is_entity_registered(entity))
		{
			auto h = SCAST(u64, entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				result = m_animatorStacks[h].get_entity_animator_as< T >(animator_name);

			);
		}

		return result;
	}


	template < class T, typename... ARGS >
	bool mint::animation::CAnimationSystem::try_push_entity_animator(entt::entity entity, const String& animator_name, ARGS&&... args)
	{
		bool result = false;

		if (is_entity_registered(entity))
		{
			auto h = SCAST(u64, entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				result = m_animatorStacks[h].try_push_entity_animator< T >(animator_name, args...);

			);
		}

		return result;
	}


	template < class T, typename... ARGS >
	bool mint::animation::CAnimationSystem::try_push_entity_animator_at_index(entt::entity entity, const String& animator_name, u64 index, ARGS&&... args)
	{
		bool result = false;

		if (is_entity_registered(entity))
		{
			auto h = SCAST(u64, entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				result = m_animatorStacks[h].try_push_entity_animator_at_index< T >(animator_name, index, args...);

			);
		}

		return result;
	}
}

#endif