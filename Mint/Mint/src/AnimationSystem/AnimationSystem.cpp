#include "AnimationSystem.h"


namespace mint::animation
{

	bool CAnimationSystem::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}

	void CAnimationSystem::terminate()
	{
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}

	void CAnimationSystem::reset()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			for (auto& as : m_animatorStacks)
			{
				as.second.terminate();
			}

		);

		m_animatorStacks.clear();
	}

	bool CAnimationSystem::request_entity_registration(entt::entity entity)
	{
		auto h = SCAST(u64, entity);
		bool result = false;

		if (!is_entity_registered(entity))
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				if (m_animatorStacks[h].initialize()) result = true;
				else m_animatorStacks.erase(h);
			);
		}

		return result;
	}

	bool CAnimationSystem::is_entity_registered(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto v = m_animatorStacks.find(h) != m_animatorStacks.end();
		
		);

		return v;
	}

	
	void CAnimationSystem::remove_entity_animator(entt::entity entity, const String& state_name, const String& animator_name)
	{
		if (is_entity_registered(entity))
		{
			auto h = SCAST(u64, entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				m_animatorStacks[h].remove_entity_animator(state_name, animator_name);

			);
		}
	}

	void CAnimationSystem::run_animation_system_thread()
	{
		_run();
	}

	void CAnimationSystem::stop_animation_system_thread()
	{
		set_should_update(false);

		_set_is_running(false);

		_wait_for_termination();
	}

	bool CAnimationSystem::is_running()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool v = m_running;

		);

		return v;
	}

	void CAnimationSystem::set_should_update(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_update = value;

		);
	}

	void CAnimationSystem::_run()
	{
		std::thread thread(&CAnimationSystem::_internal_run, this);

		thread.detach();
	}

	void CAnimationSystem::_internal_run()
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

	void CAnimationSystem::_internal_computation()
	{
		MINT_PROFILE_SCOPE("CAnimationSystem::_internal_computation", "Engine::Animation");

		set_should_update(false);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const auto active = m_animationsActive;

		);

		if (!active) return;


		f32 dt = MINT_ENGINE()->get_engine_frametime();

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			for (auto& animators : m_animatorStacks)
			{
				animators.second.on_update(dt);
			}

		);
	}

	void CAnimationSystem::_set_is_running(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_running = value;

		);
	}

	bool CAnimationSystem::_should_update()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool v = m_update;

		);

		return v;
	}

	void CAnimationSystem::_wait_for_termination()
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


	void CAnimationSystem::set_entity_animation_state(entt::entity entity, const String& state_name)
	{
		if (is_entity_registered(entity))
		{
			auto h = SCAST(u64, entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				m_animatorStacks[h].set_current_state(state_name);

			);
		}
	}

	void CAnimationSystem::add_entity_animation_state(entt::entity entity, const String& state_name)
	{
		if (is_entity_registered(entity))
		{
			auto h = SCAST(u64, entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				m_animatorStacks[h].add_animation_state(state_name);

			);
		}
	}

	void CAnimationSystem::remove_entity_animation_state(entt::entity entity, const String& state_name)
	{
		if (is_entity_registered(entity))
		{
			auto h = SCAST(u64, entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				m_animatorStacks[h].remove_animation_state(state_name);

			);
		}
	}

	mint::animation::CAnimator* CAnimationSystem::try_push_entity_animator(entt::entity entity, const String& state_name, const String& animator_name)
	{
		mint::animation::CAnimator* animator = nullptr;

		if (is_entity_registered(entity))
		{
			auto h = SCAST(u64, entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				animator = m_animatorStacks[h].try_push_animator(entity, state_name, animator_name);

			);
		}

		return animator;
	}

	void CAnimationSystem::set_all_animations_active(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_animationsActive = value;

		);
	}

}