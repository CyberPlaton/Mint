#include "TranslationAnimator.h"


namespace mint::animation
{
	namespace translationanim
	{
		bool on_animation_update(CAnimator& animator, f32 dt, void* animation_data)
		{
			MINT_ASSERT(animation_data != nullptr, "Invalid operation. Animation data was nullptr!");

			auto& data = *reinterpret_cast<STranslationAnimationBehaviorData*>(animation_data);

			animator.advance_animation_counter(dt);

			Vec2 final_position;

			if (data.m_forward)
			{
				// Interpolate scale in direction of dest color.
				auto ease = animator.get_current_easing_t_between_zero_and_one();

				final_position = glm::lerp(data.m_baseTranslation, data.m_destTranslation, ease);

				if (!mint::algorithm::are_values_sufficiently_different(final_position, data.m_destTranslation, 3.0f))
				{
					animator.set_animation_counter(0.0f);
					data.m_forward = false;
				}
			}
			else
			{
				// Interpolate scale in direction of base color.
				auto ease = animator.get_current_easing_t_between_zero_and_one();

				final_position = glm::lerp(data.m_destTranslation, data.m_baseTranslation, ease);

				if (!mint::algorithm::are_values_sufficiently_different(final_position, data.m_baseTranslation, 3.0f))
				{
					animator.set_animation_counter(0.0f);
					data.m_forward = true;
				}
			}


			CUCA::transform_set_position(animator.get_animator_entity(), final_position);

			return true;
		}

		bool on_animator_initialize(CAnimator& animator, void* animation_data)
		{
			if (!animator.is_entity_valid() || animation_data == nullptr) return false;

			return true;
		}

		void on_animator_terminate(CAnimator& animator, void* animation_data)
		{
		}

		void on_animation_enter(CAnimator& animator, void* animation_data)
		{
			MINT_ASSERT(animation_data != nullptr, "Invalid operation. Animation data was nullptr!");

			auto& data = *reinterpret_cast<STranslationAnimationBehaviorData*>(animation_data);

			auto current = CUCA::transform_get_position(animator.get_animator_entity());

			CUCA::transform_set_position(animator.get_animator_entity(), current + data.m_baseTranslation);
		}

		void on_animation_exit(CAnimator& animator, void* animation_data)
		{

		}

	}

}