#include "DebugRenderer.h"


namespace mint::fx
{
	void CDebugRenderer::_on_render_destination_rectangle(const Vector< entt::entity >& entities)
	{
		Vector< entt::entity > rendering_entities;

		if (m_renderAll)
		{
			rendering_entities = entities;
		}
		else
		{
			rendering_entities = m_filteredEntities;
		}

		for (const auto& entity : rendering_entities)
		{
			auto dest = CUCA::sprite_get_destination_rect(entity);

			CPrimitiveRenderer::RenderRectangle({ dest.get_x(), dest.get_y() }, { dest.get_width(), dest.get_height() }, MINT_BLUE_DARK(), 2.0f);
		}
	}


	void CDebugRenderer::_on_render_sprite_origin(const Vector< entt::entity >& entities)
	{
		Vector< entt::entity > rendering_entities;

		if (m_renderAll)
		{
			rendering_entities = entities;
		}	
		else
		{
			rendering_entities = m_filteredEntities;
		}


		for (const auto& entity : rendering_entities)
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

		// Render Grid if required.
		if (m_renderGrid)
		{
			rlPushMatrix();
			rlTranslatef(0, 0, 0);
			rlRotatef(-90, 1, 0, 0);
			DrawGrid(m_gridCellCount, m_gridCellSize);
			rlPopMatrix();
		}
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

	void CDebugRenderer::add_entity_to_filter(entt::entity entity)
	{
		mint::algorithm::vector_push_back(m_filteredEntities, entity);
	}

	void CDebugRenderer::remove_entity_from_filter(entt::entity entity)
	{
		mint::algorithm::vector_erase(m_filteredEntities, entity);
	}

	void CDebugRenderer::clear_entity_filter()
	{
		m_filteredEntities.clear();
	}

	void CDebugRenderer::set_render_all_entities(bool value)
	{
		m_renderAll = value;
	}

	void CDebugRenderer::set_render_grid(bool value, u64 cell_size, u64 cell_count)
	{
		m_renderGrid = value;
		m_gridCellSize = cell_size;
		m_gridCellCount = cell_count;
	}


}