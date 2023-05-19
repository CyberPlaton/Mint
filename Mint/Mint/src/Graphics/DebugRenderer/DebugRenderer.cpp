#include "DebugRenderer.h"


namespace mint::fx
{

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

			CPrimitiveRenderer::RenderCircleOutlined({ dest.get_x() + origin.x, dest.get_y() + origin.y }, 10.0f, MINT_GREEN_LIGHT(), MINT_GREEN_DARK());
		}
	}


}