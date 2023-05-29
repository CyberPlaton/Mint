#include "DebugRenderer.h"


namespace mint::fx
{
	void CDebugRenderer::_on_render_destination_rectangle(const Vector< entt::entity >& entities)
	{
		for (const auto& entity : entities)
		{
			auto dest = CUCA::sprite_get_destination_rect(entity);

			CPrimitiveRenderer::RenderRectangle({ dest.get_x(), dest.get_y() }, { dest.get_width(), dest.get_height() }, MINT_BLUE_DARK(), 2.0f);
		}
	}


	void CDebugRenderer::_on_render_sprite_origin(const Vector< entt::entity >& entities)
	{
		for (const auto& entity : entities)
		{
			auto dest = CUCA::sprite_get_destination_rect(entity);
			auto origin = CUCA::sprite_get_origin(entity);
			auto scale = CUCA::transform_get_scale(entity);

			CPrimitiveRenderer::RenderCircleOutlined({ dest.get_x() + origin.x * scale.x, dest.get_y() + origin.y * scale.y }, 10.0f, MINT_GREEN_LIGHT(), MINT_GREEN_DARK());
		}	
	}


	void CDebugRenderer::on_frame_begin(ICamera* render_camera)
	{
		// Using default camera from Rendering Pass Stack.
	}


	void CDebugRenderer::on_frame_end()
	{
	}

	void CDebugRenderer::on_frame(Vector< entt::entity >& entities)
	{
		if (m_renderDestRect) _on_render_destination_rectangle(entities);
		if (m_renderSpriteOrigin) _on_render_sprite_origin(entities);
	}

	void CDebugRenderer::set_render_destination_rectangle(bool value)
	{
		m_renderDestRect = value;
	}

	void CDebugRenderer::set_render_sprite_origin(bool value)
	{
		m_renderSpriteOrigin = value;
	}

	bool CDebugRenderer::initialize()
	{
		return true;
	}

	void CDebugRenderer::terminate()
	{

	}

	CDebugRenderer::CDebugRenderer() : CRenderingPass("CDebugRenderer")
	{

	}


}