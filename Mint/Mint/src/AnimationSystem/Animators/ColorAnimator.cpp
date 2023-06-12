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
				Vec3 base_color = { data.m_baseColor.m_r, data.m_baseColor.m_g, data.m_baseColor.m_b };
				Vec3 dest_color = { data.m_destColor.m_r, data.m_destColor.m_g, data.m_destColor.m_b };

				Vec3 color = glm::lerp(base_color, dest_color, bx::getEaseFunc(animator.get_animation_easing_function())(animator.get_animation_counter() / animator.get_animation_duration()));

				auto ec = CUCA::sprite_get_color(animator.get_animator_entity());
				Vec3 entity_color = { ec.m_r, ec.m_g, ec.m_b };

				color = glm::mix(color, entity_color, 0.5f);

				final_color.set_color(SCAST(u8, color.r), SCAST(u8, color.g), SCAST(u8, color.b), SCAST(u8, 255));



				if (ec.as_rgba() == data.m_destColor.as_rgba())
				{
					animator.set_animation_counter(0.0f);
					data.m_forward = false;
				}
			}
			else
			{
				// Interpolate color in direction of base color.
				Vec3 base_color = { data.m_baseColor.m_r, data.m_baseColor.m_g, data.m_baseColor.m_b };
				Vec3 dest_color = { data.m_destColor.m_r, data.m_destColor.m_g, data.m_destColor.m_b };

				Vec3 color = glm::lerp(dest_color, base_color, bx::getEaseFunc(animator.get_animation_easing_function())(animator.get_animation_counter() / animator.get_animation_duration()));

				auto ec = CUCA::sprite_get_color(animator.get_animator_entity());
				Vec3 entity_color = { ec.m_r, ec.m_g, ec.m_b };

				color = glm::mix(color, entity_color, 0.5f);

				final_color.set_color(SCAST(u8, color.r), SCAST(u8, color.g), SCAST(u8, color.b), SCAST(u8, 255));



				if (ec.as_rgba() == data.m_baseColor.as_rgba())
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