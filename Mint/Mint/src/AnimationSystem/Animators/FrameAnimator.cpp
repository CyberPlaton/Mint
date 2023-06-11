#include "FrameAnimator.h"

namespace mint::animation
{

	CFrameAnimator::CFrameAnimator(const String& animator_name, entt::entity entity) :
		CAnimator(animator_name, entity)
	{
	}

	bool CFrameAnimator::on_initialize()
	{
		if (!is_handle_valid(entity_get_handle(m_entity))) return false;

		auto sprite_size = CUCA::sprite_get_size(m_entity);


	}

	void CFrameAnimator::on_terminate()
	{

	}

	void CFrameAnimator::on_animation_update(f32 dt)
	{
		m_frameCounter += dt;

		f32 cursor = glm::lerp(0.0f, 1.0f, bx::getEaseFunc(get_animation_easing_function())(m_frameCounter / get_animation_duration()));
	
		if (cursor >= 1.0f)
		{
			// Advance to next Keyframe index.
			m_currentFrameIndex = (++m_currentFrameIndex) % m_keyframes.size();

			m_currentFrame = m_keyframes[m_currentFrameIndex];


			// Compute the new source rectangle and set it for the entity.
			f32 x = m_currentFrame.first * m_frameWidth;
			f32 y = m_currentFrame.second * m_frameHeight;

			f32 w = m_frameWidth;
			f32 h = m_frameHeight;

 			u32 textureWidth = m_frameWidth * m_frameCountX;
			u32 textureHeight = m_frameHeight * m_frameCountY;


			CUCA::sprite_set_source_rect_from_vec(get_animator_entity(), { x, y, w, h });

			m_frameCounter = 0.0f;
		}
	}

	void CFrameAnimator::on_animation_enter()
	{
		// Change texture for required current animator.
		mint::fx::CMaterialManager::Get().set_material_for_entity(get_animation_material(), get_animator_entity());

		mint::fx::CMaterial* material = CUCA::sprite_get_main_material(get_animator_entity());
	
		auto size = material->get_texture_dimension();

		m_frameWidth = size.x / m_frameCountX;
		m_frameHeight = size.y / m_frameCountY;

		// Set sprite texture origin and destination to match the sprite size.
		CUCA::sprite_set_origin(get_animator_entity(), { (size.x / m_frameCountX) / 2.0f, (size.y / m_frameCountY) / 2.0f });

		m_currentFrameIndex = 0;
		m_currentFrame = m_keyframes[m_currentFrameIndex];

		// Set the material dimension to be in unison with the animation.
		// This is required in order to draw only a part of the texture as destination rectangle.
		material->set_texture(material->get_texture_handle(), { m_frameWidth, m_frameHeight });


		// Perform the initial animation update.
		f32 x = m_currentFrame.first * m_frameWidth;
		f32 y = m_currentFrame.second * m_frameHeight;

		f32 w = m_frameWidth;
		f32 h = m_frameHeight;

		u32 textureWidth = m_frameWidth * m_frameCountX;
		u32 textureHeight = m_frameHeight * m_frameCountY;

		CUCA::sprite_set_source_rect_from_vec(get_animator_entity(), { x, y, w, h });
	}

	void CFrameAnimator::on_animation_exit()
	{

	}

	CFrameAnimator& CFrameAnimator::add_keyframe(MINT_ANIMATOR_FRAME_NUMBER x, MINT_ANIMATOR_FRAME_NUMBER y)
	{
		mint::algorithm::vector_push_back(m_keyframes, std::make_pair(x, y));
		
		return *this;
	}

	CFrameAnimator& CFrameAnimator::set_frame_count_x(MINT_ANIMATOR_FRAME_NUMBER x)
	{
		m_frameCountX = x;

		return *this;
	}

	CFrameAnimator& CFrameAnimator::set_frame_count_y(MINT_ANIMATOR_FRAME_NUMBER y)
	{
		m_frameCountY = y;

		return *this;
	}

}