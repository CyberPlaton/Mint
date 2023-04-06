#include "SceneRenderer.h"


namespace mint::fx
{

	mint::fx::CSceneRenderer* CSceneRenderer::s_CSceneRenderer = nullptr;


	bool CSceneRenderer::initialize()
	{
		QuadVertex::initialize();


#ifndef MINT_DISTR
 		bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif

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

		
		bgfx::touch(viewport.m_viewIdentifier);
		
		bgfx::setViewTransform(viewport.m_viewIdentifier, glm::value_ptr(view), glm::value_ptr(proj));

 		bgfx::setViewRect(viewport.m_viewIdentifier, 0, 0, viewport.m_right, viewport.m_bottom);

		constexpr uint64_t stateDefault = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL | BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA |
										  BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

		bgfx::setState(
			stateDefault
		);
	}


	void CSceneRenderer::render(SViewport& viewport, mint::IScene* scene /*= mint::IScene::get_active_scene()*/)
	{
#ifndef MINT_DISTR
		if(!scene->get_entities().empty())
		{
			m_spriteBatch.start_batch(viewport.m_viewIdentifier);


			auto view = MINT_SCENE_REGISTRY().get_registry_view< component::SSprite >();

			for(auto& entity: view)
			{
				const auto& sprite = MINT_SCENE_REGISTRY().get_component< component::SSprite >(entity);
				const auto& transform = MINT_SCENE_REGISTRY().get_component< component::STransform >(entity);


				m_spriteBatch.draw_sprite(CUCA::transform_get_world_transform_matrix(entity),
										  sprite.m_color, sprite.m_rect, sprite.m_textureHandle,
										  sprite.m_flipX, sprite.m_flipY);
			}


			m_spriteBatch.flush_batch();
		}
#endif
	}


	void CSceneRenderer::end()
	{
		bgfx::frame();
	}


}