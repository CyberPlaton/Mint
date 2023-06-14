#include "ScaleAnimator.h"


namespace mint::animation
{
	namespace scaleanim
	{
		bool on_animation_update(CAnimator& animator, f32 dt, void* animation_data)
		{
			MINT_ASSERT(animation_data != nullptr, "Invalid operation. Animation data was nullptr!");

			auto& data = *reinterpret_cast<SScaleAnimationBehaviorData*>(animation_data);

			animator.advance_animation_counter(dt);

			Vec2 final_scale;

			if (data.m_forward)
			{
				// Interpolate scale in direction of dest color.
				auto ease = animator.get_current_easing_t_between_zero_and_one();

				Vec2 scale = glm::lerp(data.m_baseScale, data.m_destScale, ease);

				auto es = CUCA::transform_get_scale(animator.get_animator_entity());

				final_scale = glm::mix(scale, es, 0.5f);

				if (!mint::algorithm::are_values_sufficiently_different(final_scale, data.m_destScale, 0.5f))
				{
					animator.set_animation_counter(0.0f);
					data.m_forward = false;
				}
			}
			else
			{
				// Interpolate scale in direction of base color.
				auto ease = animator.get_current_easing_t_between_zero_and_one();

				Vec2 scale = glm::lerp(data.m_destScale, data.m_baseScale, ease);


				auto es = CUCA::transform_get_scale(animator.get_animator_entity());

				final_scale = glm::mix(scale, es, 0.5f);

				if (!mint::algorithm::are_values_sufficiently_different(final_scale, data.m_baseScale, 0.5f))
				{
					animator.set_animation_counter(0.0f);
					data.m_forward = true;
				}
			}


			CUCA::transform_set_scale(animator.get_animator_entity(), final_scale);

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

			auto& data = *reinterpret_cast<SScaleAnimationBehaviorData*>(animation_data);

			CUCA::transform_set_scale(animator.get_animator_entity(), data.m_baseScale);
		}

		void on_animation_exit(CAnimator& animator, void* animation_data)
		{

		}

	}

}