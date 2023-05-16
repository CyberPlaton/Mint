#include "DebugRenderer.h"


namespace mint::fx
{
	void CDebugRenderer::on_render(mint::IScene* scene)
	{
	}


	void CDebugRenderer::on_render(Vector< entt::entity >& entities)
	{
		for(auto& entity : entities)
		{
			auto dest = CUCA::sprite_get_destination_rect(entity);

			CPrimitiveRenderer::RenderRectangle({ dest.get_x(), dest.get_y() }, { dest.get_width(), dest.get_height() }, MINT_BLUE_LIGHT());
		}
	}

}