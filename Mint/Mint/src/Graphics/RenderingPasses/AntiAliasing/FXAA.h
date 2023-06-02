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
		
		void on_frame_begin(ICamera* render_camera) override final;

		void on_frame_end() override final;


		f32 get_blur_threshold() { return m_blurThreshold; }

		f32 get_saturation() { return m_saturation; }

		f32 get_render_texture_offset() { return m_renderTextureOffset; }


		void set_blur_threshold(f32 value) { m_blurThreshold = value; }

		void set_saturation(f32 value) { m_saturation = value; }

		void set_render_texture_offset(f32 value) { m_renderTextureOffset = value; }


	private:
		Shader m_fxaaShader;


	private:
		f32 m_blurThreshold = 0.1f;
		f32 m_saturation = 0.5f;
		f32 m_renderTextureOffset = 0.5f;
	};
}


#endif