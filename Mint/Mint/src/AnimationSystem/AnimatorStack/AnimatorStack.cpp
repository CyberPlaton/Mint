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

		if (m_currentState.empty()) m_currentState = state_name;
	}

	CAnimator* CAnimatorStack::try_push_animator(entt::entity entity, const String& state_name, const String& animator_name)
	{
		auto h = mint::algorithm::djb_hash(animator_name);
		auto s = mint::algorithm::djb_hash(state_name);


		if (!lookup_animation_state(state_name))
		{
			add_animation_state(state_name);
		}
		

		auto animator = m_animators[s].emplace(h);

		animator->set_name(animator_name);
		animator->set_entity(entity);

		return animator;
	}

	void CAnimatorStack::remove_animation_state(const String& state_name)
	{
		auto s = mint::algorithm::djb_hash(state_name);

		m_animators.erase(s);
	}

	void CAnimatorStack::on_update(f32 dt)
	{
		MINT_PROFILE_SCOPE("CAnimatorStack::on_update", "Engine::Animation");

		if (!m_currentState.empty())
		{
			auto h = mint::algorithm::djb_hash(m_currentState);

			auto& map = m_animators[h];

			auto animator = map.begin();
			while (animator)
			{
				animator->on_animation_update(dt);

				animator = map.advance(animator);
			}
		}
	}

	void CAnimatorStack::set_current_state(const String& state_name)
	{
		m_currentState = state_name;
	}

	bool CAnimatorStack::lookup_animation_state(const String& state_name)
	{
		auto s = mint::algorithm::djb_hash(state_name);

		return m_animators.find(s) != m_animators.end();
	}

}