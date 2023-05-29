#ifndef _MINTFX_DEBUG_RENDERER_H_
#define _MINTFX_DEBUG_RENDERER_H_


#include "Components/CUCA.h"
#include "../PrimitiveRenderer/PrimitiveRenderer.h"
#include "../../Common/IRenderingPass.h"


namespace mint::fx
{

	class CDebugRenderer : public CRenderingPass
	{
	public:
		CDebugRenderer();

		bool initialize() override final;
		void terminate() override final;

		void on_frame_begin(ICamera* render_camera) override final;
		void on_frame(Vector< entt::entity >& entities)  override final;
		void on_frame_end()  override final;

		void set_render_destination_rectangle(bool value);
		void set_render_sprite_origin(bool value);
		void set_render_all_entities(bool value);
		void set_render_grid(bool value, u64 cell_size = 0, u64 cell_count = 0);

		void add_entity_to_filter(entt::entity entity);
		void remove_entity_from_filter(entt::entity entity);
		void clear_entity_filter();

	private:
		bool m_renderGrid = false;
		u64 m_gridCellSize = 0;
		u64 m_gridCellCount = 0;
		bool m_renderDestRect = false;
		bool m_renderSpriteOrigin = false;
		bool m_renderAll = false;

		Vector< entt::entity > m_filteredEntities;


	private:
		void _on_render_destination_rectangle(const Vector< entt::entity >& entities);

		void _on_render_sprite_origin(const Vector< entt::entity >& entities);

	};

}


#endif