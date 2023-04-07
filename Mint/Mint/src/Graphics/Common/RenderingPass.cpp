#include "RenderingPass.h"

namespace mint::fx
{


	IRenderingPass::IRenderingPass(ICamera* rendering_camera) : 
		m_renderingCamera(rendering_camera), m_initialized(false)
	{
	}


}