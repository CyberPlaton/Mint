#include "AnimatorStack.h"

namespace mint::animation
{
	mint::CMap< mint::animation::CAnimatorStack::AnimatorImporterFunction > CAnimatorStack::m_animatorImporters;


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

		m_animators[s].remove_node(h);
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
		auto animator = m_animators[s].add_node(h);

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
			m_animators[s].remove_node(h);
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
		MINT_PROFILE_SCOPE("Engine::Animation", "CAnimatorStack::on_update");

		if (is_handle_valid(m_currentState))
		{
			// Check whether we need to perform a state transition.
			if (m_stateChange)
			{
				// Perform on exit action only if there was a valid previous state for 
				// the old animators.
				if (is_handle_valid(m_previousState))
				{
					auto& prev = m_animators[m_previousState];

					for (auto anim = prev.begin(); anim != nullptr; anim = prev.advance(anim))
					{
						anim->on_animation_exit();
					}
				}

				// All animators of the current (new) state entering the animation.
				auto& curr = m_animators[m_currentState];
				for (auto anim = curr.begin(); anim != nullptr; anim = curr.advance(anim))
				{
					anim->on_animation_enter();
				}


				m_stateChange = false;
			}


			if (m_initialUpdatePerformed == false)
			{
				// First ever animation enter for current state animators.
				auto& curr = m_animators[m_currentState];
				for (auto anim = curr.begin(); anim != nullptr; anim = curr.advance(anim))
				{
					anim->on_animation_enter();
				}

				m_initialUpdatePerformed = true;
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

	bool CAnimatorStack::load_animation_for_entity_from_file(entt::entity entity, const String& animation_file_path)
	{
		/*
		* Animation:
		*	State0:
		*		animator="animations/anim_frame_gopnik_idle.anim"
		*		animator="animations/anim_color_gopnik_idle.anim"
		*	end
		* 
		*	State1:
		*		animator="animations/anim_frame_gopnik_walk.anim"
		*		animator="animations/anim_translation_gopnik_walk.anim"
		*	end
		* 
		* end
		*/

		MINT_LOG_INFO("[{:.4f}][CAnimatorStack::load_animation_for_entity_from_file] Importing Animation \"{}\" for entity \"{}\"", MINT_APP_TIME, animation_file_path, SCAST(u64, entity));

		maml::CDocument document(MAML_DOCUMENT_SIZE_DEFAULT);


		auto root = mint::CSerializer::load_maml_document(animation_file_path, document);

		MINT_ASSERT(root != nullptr, "Invalid operation. Failed loading animation file!");

		if (root == nullptr) return false;


		auto animation_node = document.find_first_match_in_document("animation");

		MINT_ASSERT(animation_node != nullptr, "Invalid operation. Failed locating animation node in animation file!");

		if (animation_node == nullptr) return false;


		// Retrieve the path of the animation for later use.
		CFilesystem fs(CFilesystem::get_file_parent_directory_relative_to_working_directory(animation_file_path));


		auto states = document.get_node_children(animation_node);

		// Iterate through states and create them in the animator stack.
		bool result = true;

		MINT_LOG_INFO("\tAnimation states to be loaded: {}", states.size());
		
		for (auto& state : states)
		{
			MINT_LOG_INFO("\t\tLoading state: \"{}\"", state->m_name);

			add_animation_state(state->m_name);

			auto& animators = maml::CDocument::get_all_node_properties(state);

			MINT_LOG_INFO("\t\t\tAnimators to be loaded: {}", animators.size());

			for (auto& animator : animators)
			{
				auto path = animator.cast< String >();

				CFilesystem afs(fs.get_current_directory());


				if (afs.forward(path)) result &= load_animator_for_entity_from_file(entity, state->m_name, afs.get_current_directory().as_string());
				else result = false;


				if (result) MINT_LOG_INFO("\t\t\t\tAnimator loaded at \"{}\"!", path);
				else MINT_LOG_WARN("\t\t\t\tLoading Animator at \"{}\" failed!", path);
			}
		}

	}

	bool CAnimatorStack::load_animator_for_entity_from_file(entt::entity entity, const String& state_name, const String& animator_file_path)
	{
		/*
		* Animator:
		*	type="Frame"
		*	duration=5.2
		*	easing=12
		*	speed=1.0
		* 
		*	// Animator type dependent data.
		*	framesx=16
		*	framesy=1
		*	material="mat_gopnik_idle"
		*	.
		*	.
		*	.
		* end
		*/

		maml::CDocument document;

		auto root = mint::CSerializer::load_maml_document(animator_file_path, document);

		MINT_ASSERT(root != nullptr, "Invalid operation. Failed loading animator file!");

		if (root == nullptr) return false;


		auto animator_node = document.find_first_match_in_document("animator");

		MINT_ASSERT(animator_node != nullptr, "Invalid operation. Failed locating animator node in animator file!");

		if (animator_node == nullptr) return false;


		auto type = maml::CDocument::get_string_property(animator_node, "type");
		auto animator_name = maml::CDocument::get_string_property(animator_node, "name");

		CAnimator* animator = nullptr;

		// Import type dependent animator data and set it for the animator.
		if (!type.empty())
		{
			auto h = mint::algorithm::djb_hash(type);

			if (m_animatorImporters.lookup(h))
			{
				animator = m_animatorImporters.get(h)(entity, state_name, animator_name, type, *this, animator_node);
			}
		}

		// Import default animator data.
		if (animator != nullptr)
		{
			auto easing = maml::CDocument::get_uint_property(animator_node, "easing");
			auto speed = maml::CDocument::get_float_property(animator_node, "speed");
			auto duration = maml::CDocument::get_float_property(animator_node, "duration");


			animator->set_entity(entity);
			animator->set_name(animator_name);
			animator->set_animation_duration(duration);
			animator->set_animation_easing_function((bx::Easing::Enum)easing);
			animator->set_animation_speed(speed);

			return true;
		}

		return false;
	}

	void CAnimatorStack::register_animator_importer(const String& animator_type, AnimatorImporterFunction function)
	{
		auto h = mint::algorithm::djb_hash(animator_type);

		m_animatorImporters.add(h, function);
	}

}