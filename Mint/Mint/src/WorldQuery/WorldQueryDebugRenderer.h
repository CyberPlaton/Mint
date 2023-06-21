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
	};
}


#endif