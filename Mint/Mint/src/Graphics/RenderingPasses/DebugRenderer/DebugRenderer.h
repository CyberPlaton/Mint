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

	private:
		bool m_renderDestRect = true;
		bool m_renderSpriteOrigin = true;


	private:
		void _on_render_destination_rectangle(const Vector< entt::entity >& entities);

		void _on_render_sprite_origin(const Vector< entt::entity >& entities);

	};

}


#endif