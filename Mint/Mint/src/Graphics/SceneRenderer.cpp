#include "SceneRenderer.h"


namespace mint::fx
{


	bool CSceneRenderer::initialize(SDescription& desc)
	{
		bgfx::renderFrame();


		bgfx::Init init;
		init.platformData.nwh = desc.m_nativeWindowHandle;
		init.resolution.width = desc.m_width;
		init.resolution.height = desc.m_height;
		init.resolution.reset = (desc.m_vsync == true) ? BGFX_RESET_VSYNC : BGFX_RESET_NONE;
		init.type = desc.m_api;

		if(bgfx::init(init))
		{

			return true;
		}

		return false;
	}

	void CSceneRenderer::terminate()
	{
		bgfx::shutdown();
	}


	void CSceneRenderer::begin(SViewport& viewport)
	{
		bgfx::touch(viewport.m_viewIdentifier);

		bgfx::setState(
			BGFX_STATE_DEFAULT
		);
	}


	void CSceneRenderer::render(SViewport& viewport, mint::IScene* scene /*= mint::IScene::get_active_scene()*/)
	{
#ifndef MINT_DISTR
		if(scene)
		{

		}
		else
		{
			const bgfx::Stats* stats = bgfx::getStats();
			bgfx::dbgTextClear();
			bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", 
							    stats->width, stats->height, stats->textWidth, stats->textHeight);

			bgfx::setDebug(BGFX_DEBUG_STATS);
		}
#endif
	}


	void CSceneRenderer::end()
	{
		bgfx::frame();
	}


}