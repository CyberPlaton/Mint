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
		m_renderingCamera = render_camera;

		ClearBackground(m_renderingCamera->get_clear_color().as_cliteral());

		m_renderingCamera->use_camera();
	}


	void CSceneRenderer::on_post_render()
	{
		m_renderingCamera->end_camera();
	}


	void CSceneRenderer::on_render(mint::IScene* scene)
	{
		auto& registry = scene->get_registry();
		auto& material_manager = CMaterialManager::Get();
		auto& texture_manager = CTextureManager::Get();


		for(auto& entity: registry.get_registry_view< mint::component::SSprite >())
		{
			const auto& transform = registry.get_component< mint::component::STransform >(entity);
			const auto& sprite = registry.get_component< mint::component::SSprite >(entity);


			const auto& materials = material_manager.get_materials_for_entity(entity);
		

			const auto& position = CUCA::transform_get_position(entity);
			const auto& scale = CUCA::transform_get_scale(entity);
			const auto& rotation = CUCA::transform_get_rotation(entity);


			const Rectangle& src = { sprite.m_rect.get_x(), sprite.m_rect.get_y(),
									  sprite.m_rect.get_width(), sprite.m_rect.get_height() };

			f32 px = position.x - src.width / 2.0f;
			f32 py = position.y - src.height / 2.0f;

			const Rectangle& dst = { px + sprite.m_origin.x, py + sprite.m_origin.y,
									 src.width * scale.x, src.height * scale.y };


			for(const auto& material: materials)
			{
				material.bind_dynamic_uniforms();

				material.bind_shader();

				material.bind_blend_mode();


				DrawTexturePro(texture_manager.get_texture(material.get_texture_handle()), src, dst, {sprite.m_origin.x, sprite.m_origin.y}, transform.m_rotation, sprite.m_color.as_cliteral());


				material.end_blend_mode();

				material.end_shader();
			}
		}
	}


	void CSceneRenderer::on_render(Vector< entt::entity >& entities)
	{

	}


	void CSceneRenderer::on_resize(ICamera* render_camera)
	{
	}

}