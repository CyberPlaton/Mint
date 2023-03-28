#include "SceneRenderer.h"


namespace mint::fx
{

	mint::fx::CSceneRenderer* CSceneRenderer::s_CSceneRenderer = nullptr;


	bool CSceneRenderer::initialize()
	{
		return true;
	}

	void CSceneRenderer::terminate()
	{
		bgfx::shutdown();
	}


	void CSceneRenderer::begin(SViewport& viewport)
	{
		auto view = MINT_ACTIVE_SCENE()->get_active_camera().get_view_matrix();
		auto proj = MINT_ACTIVE_SCENE()->get_active_camera().get_project_matrix();

		bgfx::setViewTransform(viewport.m_viewIdentifier, &view[0], &proj[0]);

		bgfx::touch(viewport.m_viewIdentifier);

		bgfx::setState(
			BGFX_STATE_DEFAULT
		);
	}


	void CSceneRenderer::render(SViewport& viewport, mint::IScene* scene /*= mint::IScene::get_active_scene()*/)
	{
#ifndef MINT_DISTR
		if(!scene->get_entities().empty())
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