#include "ColorAnimator.h"


namespace mint::animation
{
	namespace coloranim
	{
		bool on_animation_update(CAnimator& animator, f32 dt, void* animation_data)
		{
			MINT_ASSERT(animation_data != nullptr, "Invalid operation. Animation data was nullptr!");

			auto& data = *reinterpret_cast<SColorAnimationBehaviorData*>(animation_data);

			animator.advance_animation_counter(dt);

			fx::CColor final_color;

			if (data.m_forward)
			{
				// Interpolate color in direction of dest color.
				Vec4 base_color = { data.m_baseColor.m_r, data.m_baseColor.m_g, data.m_baseColor.m_b, data.m_baseColor.m_a };
				Vec4 dest_color = { data.m_destColor.m_r, data.m_destColor.m_g, data.m_destColor.m_b, data.m_destColor.m_a };

				auto ease = animator.get_current_easing_t_between_zero_and_one();

				Vec4 color = glm::lerp(base_color, dest_color, ease);


				auto ec = CUCA::sprite_get_color(animator.get_animator_entity());

				Vec4 entity_color = { ec.m_r, ec.m_g, ec.m_b, ec.m_a };

				color = glm::mix(color, entity_color, 0.5f);

				final_color.set_color(SCAST(u8, color.r) % 256, SCAST(u8, color.g) % 256, SCAST(u8, color.b) % 256, SCAST(u8, color.a) % 256);


				if (final_color == data.m_destColor)
				{
					animator.set_animation_counter(0.0f);
					data.m_forward = false;
				}
			}
			else
			{
				// Interpolate color in direction of base color.
				Vec4 base_color = { data.m_baseColor.m_r, data.m_baseColor.m_g, data.m_baseColor.m_b, data.m_baseColor.m_a };
				Vec4 dest_color = { data.m_destColor.m_r, data.m_destColor.m_g, data.m_destColor.m_b, data.m_destColor.m_a };

				auto ease = animator.get_current_easing_t_between_zero_and_one();

				Vec4 color = glm::lerp(dest_color, base_color, ease);


				auto ec = CUCA::sprite_get_color(animator.get_animator_entity());

				Vec4 entity_color = { ec.m_r, ec.m_g, ec.m_b, ec.m_a };

				color = glm::mix(color, entity_color, 0.5f);

				final_color.set_color(SCAST(u8, color.r) % 256, SCAST(u8, color.g) % 256, SCAST(u8, color.b) % 256, SCAST(u8, color.a) % 256);


				if (final_color == data.m_baseColor)
				{
					animator.set_animation_counter(0.0f);
					data.m_forward = true;
				}
			}


			CUCA::sprite_set_color(animator.get_animator_entity(), final_color);

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

			auto& data = *reinterpret_cast<SColorAnimationBehaviorData*>(animation_data);

			CUCA::sprite_set_color(animator.get_animator_entity(), data.m_baseColor);
		}

		void on_animation_exit(CAnimator& animator, void* animation_data)
		{

		}

	}

}