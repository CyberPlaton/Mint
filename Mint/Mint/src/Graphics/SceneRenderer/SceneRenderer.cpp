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
	}

	void CSceneRenderer::on_pre_render(ICamera* render_camera)
	{
		m_currentRenderCamera = render_camera;
	}


	void CSceneRenderer::on_post_render()
	{
	}


	void CSceneRenderer::on_render(mint::IScene* scene)
	{

	}


	void CSceneRenderer::on_render(Vector< entt::entity >& entities)
	{

	}


	void CSceneRenderer::on_resize(ICamera* render_camera)
	{
	}

}