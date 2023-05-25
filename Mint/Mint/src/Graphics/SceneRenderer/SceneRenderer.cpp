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
			MINT_ASSERT(registry.has_component< mint::component::SSprite >(entity) == true,
				"Invalid operation. Trying to render an Entity without a Sprite Component!");

			const auto& transform = registry.get_component< mint::component::STransform >(entity);
			const auto& sprite = registry.get_component< mint::component::SSprite >(entity);


			auto& materials = material_manager.get_materials_for_entity(entity);
		
			
			const auto& position = CUCA::transform_get_position(entity);
			const auto& scale = CUCA::transform_get_scale(entity);
			const auto& rotation = CUCA::transform_get_rotation(entity);


			auto material = materials.begin();
			while (material)
			{

				material->bind_dynamic_uniforms();

				material->bind_shader();

				material->bind_blend_mode();

				const Vec2& texture_size = material->get_texture_dimension();

				const Rectangle& src = { sprite.m_rect.get_x(), sprite.m_rect.get_y(),
										 sprite.m_rect.get_width() * texture_size.x, sprite.m_rect.get_height() * texture_size.y };

				f32 px = position.x - src.width / 2.0f;
				f32 py = position.y - src.height / 2.0f;

				const Rectangle& dst = { px + sprite.m_origin.x, py + sprite.m_origin.y,
										 src.width * scale.x, src.height * scale.y };



				DrawTexturePro(texture_manager.get_texture(material->get_texture_handle()), src, dst, { sprite.m_origin.x * scale.x, sprite.m_origin.y * scale.y }, mint::algorithm::radians_to_degree(rotation), sprite.m_color.as_cliteral());


				material->end_blend_mode();

				material->end_shader();


				material = materials.advance(material);
			}
		}
	}


	void CSceneRenderer::on_render(Vector< entt::entity >& entities)
	{
		auto& registry = MINT_ACTIVE_SCENE()->get_registry();
		auto& material_manager = CMaterialManager::Get();
		auto& texture_manager = CTextureManager::Get();


		for (auto& entity : entities)
		{
			MINT_ASSERT(registry.has_component< mint::component::SSprite >(entity) == true,
					   "Invalid operation. Trying to render an Entity without a Sprite Component!");


			const auto& transform = registry.get_component< mint::component::STransform >(entity);
			const auto& sprite = registry.get_component< mint::component::SSprite >(entity);


			auto& materials = material_manager.get_materials_for_entity(entity);

			MINT_LOG_INFO("[{:.4f}][CSceneRenderer::on_render] Entity Materials Vector size: \"{}\" Bytes!", MINT_APP_TIME, sizeof(materials));

			const auto& position = CUCA::transform_get_position(entity);
			const auto& scale = CUCA::transform_get_scale(entity);
			const auto& rotation = CUCA::transform_get_rotation(entity);


			auto material = materials.begin();
			while (material)
			{

				material->bind_dynamic_uniforms();

				material->bind_shader();

				material->bind_blend_mode();

				const Vec2& texture_size = material->get_texture_dimension();

				const Rectangle& src = { sprite.m_rect.get_x(), sprite.m_rect.get_y(),
										 sprite.m_rect.get_width() * texture_size.x, sprite.m_rect.get_height() * texture_size.y };

				f32 px = position.x - src.width / 2.0f;
				f32 py = position.y - src.height / 2.0f;

				const Rectangle& dst = { px + sprite.m_origin.x, py + sprite.m_origin.y,
										 src.width * scale.x, src.height * scale.y };



				DrawTexturePro(texture_manager.get_texture(material->get_texture_handle()), src, dst, { sprite.m_origin.x * scale.x, sprite.m_origin.y * scale.y }, mint::algorithm::radians_to_degree(rotation), sprite.m_color.as_cliteral());


				material->end_blend_mode();

				material->end_shader();


				material = materials.advance(material);
			}
		}
	}


	void CSceneRenderer::on_resize(ICamera* render_camera)
	{
	}

}