#include "IRenderingPass.h"

namespace mint::fx
{

	void CRenderingPass::on_frame_begin(ICamera* render_camera)
	{
		// Using default camera set in Rendering Pass Stack.
	}

	void CRenderingPass::on_resize(ICamera* render_camera)
	{
		UnloadRenderTexture(m_renderTexture);

		m_renderTexture = LoadRenderTexture(render_camera->get_viewport_width(), render_camera->get_viewport_height());
	
		m_renderCamera = render_camera;
	}

	CRenderingPass::CRenderingPass(const String& rendering_pass_name) :
		m_renderingPassName(rendering_pass_name)
	{

	}

}