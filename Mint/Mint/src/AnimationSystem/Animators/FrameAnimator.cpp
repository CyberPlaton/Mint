#include "FrameAnimator.h"

namespace mint::animation
{
	namespace frameanim
	{

		mint::animation::frameanim::SFrameAnimationBehaviorData& SFrameAnimationBehaviorData::add_keyframe(MINT_ANIMATOR_FRAME_NUMBER x, MINT_ANIMATOR_FRAME_NUMBER y)
		{
			mint::algorithm::vector_push_back(m_keyframes, std::make_pair(x, y));

			return *this;
		}

		mint::animation::frameanim::SFrameAnimationBehaviorData& SFrameAnimationBehaviorData::set_frame_count_x(MINT_ANIMATOR_FRAME_NUMBER x)
		{
			m_frameCountX = x;

			return *this;
		}

		mint::animation::frameanim::SFrameAnimationBehaviorData& SFrameAnimationBehaviorData::set_frame_count_y(MINT_ANIMATOR_FRAME_NUMBER y)
		{
			m_frameCountY = y;

			return *this;
		}

		bool on_animator_initialize(CAnimator& animator, void* animation_data)
		{
			if (!animator.is_entity_valid() || animation_data == nullptr) return false;

			return true;
		}

		void on_animator_terminate(CAnimator& animator, void* animation_data)
		{

		}


		bool on_animation_update(CAnimator& animator, f32 dt, void* animation_data)
		{
			MINT_ASSERT(animation_data != nullptr, "Invalid operation. Animation data was nullptr!");

			MINT_PROFILE_SCOPE("Engine::Animation", "frameanim::on_animation_update");


			auto& data = *reinterpret_cast<SFrameAnimationBehaviorData*>(animation_data);

			// Advance counter on each frame.
			animator.advance_animation_counter(dt);

			auto ease = animator.get_current_easing_t_between_zero_and_one();

			f32 cursor = glm::lerp(0.0f, 1.0f, ease);

			if (cursor >= 1.0f)
			{
				// Advance to next Keyframe index.
				data.m_currentFrameIndex = (++data.m_currentFrameIndex) % data.m_keyframes.size();

				data.m_currentFrame = data.m_keyframes[data.m_currentFrameIndex];


				// Compute the new source rectangle and set it for the entity.
				f32 x = data.m_currentFrame.first * data.m_frameWidth;
				f32 y = data.m_currentFrame.second * data.m_frameHeight;

				f32 w = data.m_frameWidth;
				f32 h = data.m_frameHeight;

				u32 textureWidth = data.m_frameWidth * data.m_frameCountX;
				u32 textureHeight = data.m_frameHeight * data.m_frameCountY;


				CUCA::sprite_set_source_rect_from_vec(animator.get_animator_entity(), { x, y, w, h });

				animator.set_animation_counter(0.0f);
			}

			return true;
		}

		void on_animation_enter(CAnimator& animator, void* animation_data)
		{
			MINT_ASSERT(animation_data != nullptr, "Invalid operation. Animation data was nullptr!");

			MINT_PROFILE_SCOPE("Engine::Animation", "frameanim::on_animation_enter");


			auto& data = *reinterpret_cast<SFrameAnimationBehaviorData*>(animation_data);

			// Change texture for required current animator.
			CUCA::sprite_set_material_at_index(animator.get_animator_entity(), animator.get_animation_material(), 0);

			mint::fx::CMaterial* material = CUCA::sprite_get_main_material(animator.get_animator_entity());

			auto size = material->get_texture_dimension();

			data.m_frameWidth = size.x / data.m_frameCountX;
			data.m_frameHeight = size.y / data.m_frameCountY;

			// Set sprite texture origin and destination to match the sprite size.
			CUCA::sprite_set_origin(animator.get_animator_entity(), { (size.x / data.m_frameCountX) / 2.0f, (size.y / data.m_frameCountY) / 2.0f });

			data.m_currentFrameIndex = 0;
			data.m_currentFrame = data.m_keyframes[data.m_currentFrameIndex];

			// Set the material dimension to be in unison with the animation.
			// This is required in order to draw only a part of the texture as destination rectangle.
			material->set_texture(material->get_texture_handle(), { data.m_frameWidth, data.m_frameHeight });


			// Perform the initial animation update.
			f32 x = data.m_currentFrame.first * data.m_frameWidth;
			f32 y = data.m_currentFrame.second * data.m_frameHeight;

			f32 w = data.m_frameWidth;
			f32 h = data.m_frameHeight;

			u32 textureWidth = data.m_frameWidth * data.m_frameCountX;
			u32 textureHeight = data.m_frameHeight * data.m_frameCountY;

			CUCA::sprite_set_source_rect_from_vec(animator.get_animator_entity(), { x, y, w, h });
		}

		void on_animation_exit(CAnimator& animator, void* animation_data)
		{

		}

	}
}