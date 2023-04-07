#ifndef _MINTFX_RENDERING_PASS_H_
#define _MINTFX_RENDERING_PASS_H_


#include "Camera.h"
#include "Common/Window.h"
#include "Viewport.h"
#include "QuadBuffer.h"


namespace mint::fx
{

	class IRenderingPass
	{
	public:
		IRenderingPass(ICamera* rendering_camera);


		virtual void initialize() {}

		virtual bool is_initialized() { return m_initialized; }

		virtual void reset() {}


		virtual void on_pre_render() {}

		virtual void on_render() {}

		virtual void on_post_render() {}

		virtual void on_resize() {}


		virtual bool has_backbuffer() { return false; }

		virtual bgfx::FrameBufferHandle get_backbuffer() { bgfx::FrameBufferHandle fbh; fbh.idx = bgfx::kInvalidHandle; return fbh; }
	

	protected:
		bool m_initialized;

		ICamera* m_renderingCamera;
	};


}


#endif