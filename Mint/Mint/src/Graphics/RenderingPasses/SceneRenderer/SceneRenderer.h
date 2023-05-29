#ifndef _MINTFX_SCENE_RENDERER_H_
#define _MINTFX_SCENE_RENDERER_H_


#include "../../Common/IRenderingPass.h"


namespace mint::fx
{

	class CSceneRenderer : public CRenderingPass
	{
	public:
		CSceneRenderer();

		bool initialize() override final;
		void terminate() override final;
		
		void on_frame_begin(ICamera* render_camera) override final;
		void on_frame(Vector< entt::entity >& entities)  override final;
		void on_frame_end()  override final;
	};

}


#endif