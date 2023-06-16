#include "AnimationSystem.h"


namespace mint::animation
{

	bool CAnimationSystem::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		CAnimatorStack::register_animator_importer("Frameanimator", frameanim::importer);
		CAnimatorStack::register_animator_importer("Coloranimator", coloranim::importer);
		CAnimatorStack::register_animator_importer("Scaleanimator", scaleanim::importer);
		CAnimatorStack::register_animator_importer("Translationanimator", translationanim::importer);

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


	void CAnimationSystem::set_all_animations_active(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_animationsActive = value;

		);
	}

	bool CAnimationSystem::are_animations_active()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const bool v = m_animationsActive;

		);

		return v;
	}

	void CAnimationSystem::create_animation_prefab(const String& animation_name, const String& animation_file_path)
	{
		auto h = mint::algorithm::djb_hash(animation_name);

		m_animationPrefabs.add(h, std::make_pair(animation_name.c_str(), animation_file_path.c_str()));
	}

	bool CAnimationSystem::set_entity_animation(entt::entity entity, const String& animation_name)
	{
		auto h = SCAST(u64, entity);
		auto hh = mint::algorithm::djb_hash(animation_name);

		if (is_entity_registered(entity) && m_animationPrefabs.lookup(hh))
		{
			auto path = m_animationPrefabs.get(hh).second;

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				const bool v = m_animatorStacks[h].load_animation_for_entity_from_file(entity, path);

			);

			return v;
		}

		return false;
	}


	namespace frameanim
	{

		mint::animation::CAnimator* importer(entt::entity entity, const String& state_name, const String& animator_name, const String& animator_type, CAnimatorStack& stack, maml::SNode* node)
		{
			/*
			* material="mat_gopnik_idle"
			* framesx=n
			* framesy=m
			* keyframes=[1 1, 1 2, 1 3, ...]
			*/

			auto data = new SFrameAnimationBehaviorData();

			auto animator = stack.try_push_animator(entity, state_name, animator_name, data,
													mint::animation::frameanim::on_animation_update,
													mint::animation::frameanim::on_animation_enter,
													mint::animation::frameanim::on_animation_exit,
													mint::animation::frameanim::on_animator_initialize,
													mint::animation::frameanim::on_animator_terminate);

			if (animator == nullptr)
			{
				stack.remove_entity_animator(state_name, animator_name);

				return nullptr;
			}

			auto material = maml::CDocument::get_string_property(node, "material");
			auto keyframes = maml::CDocument::get_array_property(node, "keyframes");
			auto framesx = maml::CDocument::get_uint_property(node, "framesx");
			auto framesy = maml::CDocument::get_uint_property(node, "framesy");

			data->set_frame_count_x(framesx);
			data->set_frame_count_y(framesy);

			for (auto& any : keyframes)
			{
				auto keyframe = any.cast< Vec2 >();

				data->add_keyframe(keyframe.x, keyframe.y);
			}

			animator->set_animation_material(material);

			return animator;
		}
	}

	namespace coloranim
	{

		mint::animation::CAnimator* importer(entt::entity entity, const String& state_name, const String& animator_name, const String& animator_type, CAnimatorStack& stack, maml::SNode* node)
		{
			/*
			* basecolor=255 255 0 255
			* destcolor=255 0 0 255
			*/

			auto data = new SColorAnimationBehaviorData();

			auto animator = stack.try_push_animator(entity, state_name, animator_name, data,
													mint::animation::coloranim::on_animation_update,
													mint::animation::coloranim::on_animation_enter,
													mint::animation::coloranim::on_animation_exit,
													mint::animation::coloranim::on_animator_initialize,
													mint::animation::coloranim::on_animator_terminate);
		
			if (animator == nullptr)
			{
				stack.remove_entity_animator(state_name, animator_name);

				return nullptr;
			}

			auto basecolor = maml::CDocument::get_vector4_property(node, "basecolor");
			auto destcolor = maml::CDocument::get_vector4_property(node, "destcolor");

			data->set_base_color(basecolor);
			data->set_destination_color(destcolor);

			return animator;
		}

	}

	namespace scaleanim
	{

		mint::animation::CAnimator* importer(entt::entity entity, const String& state_name, const String& animator_name, const String& animator_type, CAnimatorStack& stack, maml::SNode* node)
		{
			/*
			* basescale=1.0 2.5
			* destcolor=0.5 0.5
			*/

			auto data = new SScaleAnimationBehaviorData();

			auto animator = stack.try_push_animator(entity, state_name, animator_name, data,
													mint::animation::scaleanim::on_animation_update,
													mint::animation::scaleanim::on_animation_enter,
													mint::animation::scaleanim::on_animation_exit,
													mint::animation::scaleanim::on_animator_initialize,
													mint::animation::scaleanim::on_animator_terminate);

			if (animator == nullptr)
			{
				stack.remove_entity_animator(state_name, animator_name);

				return nullptr;
			}

			auto basescale = maml::CDocument::get_vector2_property(node, "basescale");
			auto destscale = maml::CDocument::get_vector2_property(node, "destscale");

			data->set_base_scale(basescale);
			data->set_destination_scale(destscale);

			return animator;
		}

	}

	namespace translationanim
	{

		mint::animation::CAnimator* importer(entt::entity entity, const String& state_name, const String& animator_name, const String& animator_type, CAnimatorStack& stack, maml::SNode* node)
		{
			/*
			* basepos=0.0 0.0
			* destpos=2500.2 1000.0
			*/

			auto data = new STranslationAnimationBehaviorData();

			auto animator = stack.try_push_animator(entity, state_name, animator_name, data,
													mint::animation::translationanim::on_animation_update,
													mint::animation::translationanim::on_animation_enter,
													mint::animation::translationanim::on_animation_exit,
													mint::animation::translationanim::on_animator_initialize,
													mint::animation::translationanim::on_animator_terminate);

			if (animator == nullptr)
			{
				stack.remove_entity_animator(state_name, animator_name);

				return nullptr;
			}

			auto basepos = maml::CDocument::get_vector2_property(node, "basepos");
			auto destpos = maml::CDocument::get_vector2_property(node, "destpos");

			data->set_base_translation(basepos);
			data->set_destination_translation(destpos);

			return animator;
		}

	}

}