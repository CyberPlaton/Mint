#include "DebugRenderer.h"


namespace mint::fx
{
	mint::fx::ICamera* CDebugRenderer::m_camera = nullptr;

	void CDebugRenderer::on_render_destination_rectangle(const Vector< entt::entity >& entities)
	{
		for (const auto& entity : entities)
		{
			auto dest = CUCA::sprite_get_destination_rect(entity);

			CPrimitiveRenderer::RenderRectangle({ dest.get_x(), dest.get_y() }, { dest.get_width(), dest.get_height() }, MINT_BLUE_DARK(), 2.0f);
		}
	}


	void CDebugRenderer::on_render_sprite_origin(const Vector< entt::entity >& entities)
	{
		for (const auto& entity : entities)
		{
			auto dest = CUCA::sprite_get_destination_rect(entity);
			auto origin = CUCA::sprite_get_origin(entity);
			auto scale = CUCA::transform_get_scale(entity);

			CPrimitiveRenderer::RenderCircleOutlined({ dest.get_x() + origin.x * scale.x, dest.get_y() + origin.y * scale.y }, 10.0f, MINT_GREEN_LIGHT(), MINT_GREEN_DARK());
		}	
	}


	void CDebugRenderer::begin(fx::ICamera* camera)
	{
		m_camera = camera;

		m_camera->use_camera();
	}


	void CDebugRenderer::end()
	{
		m_camera->end_camera();
	}


}