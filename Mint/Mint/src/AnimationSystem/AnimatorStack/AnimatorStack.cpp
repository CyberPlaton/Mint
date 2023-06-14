#include "AnimatorStack.h"

namespace mint::animation
{

	bool CAnimatorStack::initialize()
	{
		return true; 
	}

	void CAnimatorStack::terminate()
	{
		m_animators.clear();
	}


	void CAnimatorStack::remove_entity_animator(const String& state_name, const String& animator_name)
	{
		auto h = mint::algorithm::djb_hash(animator_name);
		auto s = mint::algorithm::djb_hash(state_name);

		m_animators[s].remove(h);
	}

	void CAnimatorStack::add_animation_state(const String& state_name)
	{
		auto h = mint::algorithm::djb_hash(state_name);

		m_animators[h].initialize(MINT_ANIMATORS_COUNT_MAX);

		if (!is_handle_valid(m_currentState)) m_currentState = h;
	}

	CAnimator* CAnimatorStack::try_push_animator(entt::entity entity, const String& state_name, const String& animator_name, void* animator_data,
		CAnimator::Animator_on_animation_update update_function,
		CAnimator::Animator_on_animation_enter enter_function,
		CAnimator::Animator_on_animation_exit exit_function,
		CAnimator::Animator_on_animator_initialize initialize_function,
		CAnimator::Animator_on_animator_terminate terminate_function)
	{
		auto h = mint::algorithm::djb_hash(animator_name);
		auto s = mint::algorithm::djb_hash(state_name);


		if (!lookup_animation_state(state_name))
		{
			add_animation_state(state_name);
		}
		

		// Create the Animator and initialize it.
		auto animator = m_animators[s].emplace(h);

		animator->set_name(animator_name);

		animator->set_entity(entity);
		
		animator->set_animator_animation_data(animator_data);

		animator->set_on_animator_initialize_function(initialize_function);
		animator->set_on_animator_terminate_function(terminate_function);
		animator->set_on_animation_enter_function(enter_function);
		animator->set_on_animation_update_function(update_function);
		animator->set_on_animation_exit_function(exit_function);

		if (!animator->on_initialize())
		{
			m_animators[s].remove(h);
			animator = nullptr;
		}

		return animator;
	}

	void CAnimatorStack::remove_animation_state(const String& state_name)
	{
		if (lookup_animation_state(state_name))
		{
			auto s = mint::algorithm::djb_hash(state_name);

			// All animators in that state must be terminated first.
			auto& animators = m_animators[s];
			for (auto anim = animators.begin(); anim != nullptr; anim = animators.advance(anim))
			{
				anim->on_terminate();
			}

			m_animators.erase(s);
		}
	}

	void CAnimatorStack::on_update(f32 dt)
	{
		MINT_PROFILE_SCOPE("CAnimatorStack::on_update", "Engine::Animation");

		if (is_handle_valid(m_currentState))
		{

			// Check whether we need to perform a state transition.
			if (m_stateChange)
			{
				// Perform on exit action only if there was a valid previous state.
				if (is_handle_valid(m_previousState))
				{
					auto& prev = m_animators[m_previousState];

					for (auto anim = prev.begin(); anim != nullptr; anim = prev.advance(anim))
					{
						anim->on_animation_exit();
					}
				}


				// All current animators entering the animation.
				auto& curr = m_animators[m_currentState];
				for (auto anim = curr.begin(); anim != nullptr; anim = curr.advance(anim))
				{
					anim->on_animation_enter();
				}


				m_stateChange = false;
			}

			auto& map = m_animators[m_currentState];

			for (auto anim = map.begin(); anim != nullptr; anim = map.advance(anim))
			{
				anim->on_animation_update(dt);
			}
		}
	}

	bool CAnimatorStack::set_current_state(const String& state_name)
	{
		if (lookup_animation_state(state_name))
		{
			m_previousState = m_currentState;
			m_currentState = mint::algorithm::djb_hash(state_name);
			m_stateChange = true;

			return true;
		}

		return false;
	}

	bool CAnimatorStack::lookup_animation_state(const String& state_name)
	{
		auto s = mint::algorithm::djb_hash(state_name);

		return m_animators.find(s) != m_animators.end();
	}

}