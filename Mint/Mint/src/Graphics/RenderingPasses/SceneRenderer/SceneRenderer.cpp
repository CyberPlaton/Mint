#include "SceneRenderer.h"


namespace mint::fx
{

	bool CSceneRenderer::initialize()
	{
		return true;
	}

	void CSceneRenderer::terminate()
	{
	}

	void CSceneRenderer::on_frame_begin(ICamera* render_camera)
	{
		// Using default camera defined in Rendering Pass Stack.
	}


	void CSceneRenderer::on_frame_end()
	{
	}


	void CSceneRenderer::on_frame(Vector< entt::entity >& entities)
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

// 				const Rectangle& src = { sprite.m_rect.get_x() * texture_size.x,	 sprite.m_rect.get_y() * texture_size.y,
//  										 sprite.m_rect.get_width() * texture_size.x, sprite.m_rect.get_height() * texture_size.y };

// 				const Rectangle& src = { sprite.m_rect.get_x(),	 sprite.m_rect.get_y(),
//   										 sprite.m_rect.get_width() * texture_size.x, sprite.m_rect.get_height() * texture_size.y };

 				const Rectangle& src = { sprite.m_rect.get_x(), sprite.m_rect.get_y(),
										 sprite.m_rect.get_width(), sprite.m_rect.get_height() };
 
 				
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

	CSceneRenderer::CSceneRenderer() : CRenderingPass("CSceneRenderer")
	{

	}

}