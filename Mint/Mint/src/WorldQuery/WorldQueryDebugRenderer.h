#ifndef _MINT_WORLD_QUERY_DEBUG_RENDERER_H_
#define _MINT_WORLD_QUERY_DEBUG_RENDERER_H_


#include "WorldQuery.h"
#include "Graphics/Common/IRenderingPass.h"
#include "Graphics/RenderingPasses/PrimitiveRenderer/PrimitiveRenderer.h"


namespace mint
{
	class CWorldQueryDebugRender : public mint::fx::CRenderingPass
	{
	public:
		CWorldQueryDebugRender();

		bool initialize() override final;
		void terminate() override final;

		void on_frame_begin(fx::ICamera* render_camera) override final;
		void on_frame(Vector< entt::entity >& entities)  override final;
		void on_frame_end()  override final;


		void set_render_aabbs(bool value) { m_renderAABBs = value; }
		void set_render_full_information(bool value) { m_renderFullInformation = value; }
		void set_aabb_color(const mint::fx::CColor& color) { m_aabbColor = color; }
		void set_render_all_entities(bool value);
		void add_entity_to_filter(entt::entity entity);
		void clear_entity_filter();

	private:
		bool m_renderAll = false;
		entt::entity m_entityFilter = entt::null;
		bool m_renderAABBs = false;
		bool m_renderFullInformation = false;
		mint::fx::CColor m_aabbColor;
	};

}


#endif