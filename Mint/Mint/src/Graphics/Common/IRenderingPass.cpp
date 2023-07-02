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

	RenderTexture2D* CRenderingPass::__get_default_rendering_texture()
	{
		MINT_ASSERT(m_defaultRenderTexture != nullptr, "Invalid operation. Default RenderTexture was nullptr!");

		return m_defaultRenderTexture;
	}

	void CRenderingPass::__set_default_rendering_texture(RenderTexture2D* texture)
	{
		MINT_ASSERT(texture != nullptr, "Invalid operation. RenderTexture was nullptr!");

		m_defaultRenderTexture = texture;
	}

	void CRenderingPass::set_rendering_pass_name(const String& rendering_pass_name)
	{
		m_renderingPassName = rendering_pass_name;
	}

}