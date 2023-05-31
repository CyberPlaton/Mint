#ifndef _MINTFX_FXAA_H_
#define _MINTFX_FXAA_H_


#include "../../Common/IRenderingPass.h"


namespace mint::fx
{
	class CFXAA : public CRenderingPass
	{
	public:
		CFXAA();

		bool initialize() override final;
		void terminate() override final;
		void on_resize(ICamera* render_camera) override final;

		bool uses_render_texture() override final { return true; }
		
		void on_frame_end() override final;


	private:
		Shader m_fxaaShader;

	};
}


#endif