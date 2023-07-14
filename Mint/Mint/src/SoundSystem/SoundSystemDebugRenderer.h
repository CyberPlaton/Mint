#ifndef _MINTSOUND_SOUND_SYSTEM_DEBUG_RENDERER_H_
#define _MINTSOUND_SOUND_SYSTEM_DEBUG_RENDERER_H_


#include "Components/CUCA.h"
#include "Graphics/Common/IRenderingPass.h"
#include "Graphics/RenderingPasses/PrimitiveRenderer/PrimitiveRenderer.h"


namespace mint
{
	class CSoundSystemDebugRender : public mint::fx::CRenderingPass
	{
	public:
		CSoundSystemDebugRender();

		bool initialize() override final;
		void terminate() override final;

		void on_frame_begin(fx::ICamera* render_camera) override final;
		void on_frame(Vector< entt::entity >& entities)  override final;
		void on_frame_end()  override final;


		void set_render_position(bool value) { m_renderPosition = value; }
		void set_render_cone(bool value) { m_renderCone = value; }
		void set_circle_color(const mint::fx::CColor& color) { m_color = color; }

		void set_render_all_entities(bool value) { m_renderAll = value; }
		void add_entity_to_filter(entt::entity entity);
		void clear_entity_filter();

		
	private:
		bool m_renderAll = false;
		entt::entity m_entityFilter = entt::null;
		bool m_renderPosition = false;
		bool m_renderCone = false;
		mint::fx::CColor m_color;
	};

}


#endif