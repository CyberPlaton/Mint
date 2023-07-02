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
		auto& registry = *MINT_ACTIVE_SCENE()->get_registry();
		auto& material_manager = CMaterialManager::Get();
		auto& texture_manager = CTextureManager::Get();


		Vec2 position = { 0.0f, 0.0f };
		Vec2 scale = { 0.0f, 0.0f };
		f32 rotation = 0.0f;


		for (auto& entity : entities)
		{
			MINT_ASSERT(registry.has_component< mint::component::SSprite >(entity) == true,
					   "Invalid operation. Trying to render an Entity without a Sprite Component!");

			const auto& sprite = registry.get_component< mint::component::SSprite >(entity);


			// Ignore particles. They are handled elsewhere. Alsoo ignore invisible sprites.
			if (!sprite.m_internalVisible || registry.has_component< mint::component::SParticleEmitter >(entity)) continue;


			const auto& transform = registry.get_component< mint::component::STransform >(entity);

			auto& materials = material_manager.get_materials_for_entity(entity);


			position = CUCA::transform_get_position(entity);
			scale = CUCA::transform_get_scale(entity);
			rotation = CUCA::transform_get_rotation(entity);


			f32 px = 0.0f;
			f32 py = 0.0f;
			Rectangle dst = { 0.0f, 0.0f, 0.0f, 0.0f };
			Vec2 texture_size = { 0.0f, 0.0f };
			Rectangle src = { 0.0f, 0.0f, 0.0f, 0.0f };


			auto material = materials.begin();
			while (material)
			{

				material->bind_dynamic_uniforms();

				material->bind_shader();

				material->bind_blend_mode();


				texture_size = material->get_texture_dimension();


 				src = { sprite.m_rect.get_x(),		sprite.m_rect.get_y(),
						sprite.m_rect.get_width(),	sprite.m_rect.get_height() };
 
				px = position.x - src.width / 2.0f;
				py = position.y - src.height / 2.0f;

				dst = { px + sprite.m_origin.x, py + sprite.m_origin.y,
 						src.width * scale.x,	src.height * scale.y };

				
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