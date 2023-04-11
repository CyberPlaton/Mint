#include "SceneRenderer.h"


namespace mint::fx
{

	mint::fx::CSceneRenderer* CSceneRenderer::s_CSceneRenderer = nullptr;


	bool CSceneRenderer::initialize()
	{
		QuadVertex::initialize();
		ScreenQuadVertex::initialize();


#ifndef MINT_DISTR
 		//bgfx::setDebug(BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS);
#endif


		m_currentTexture = BGFX_INVALID_HANDLE;
		m_currentTextureSize = { 0.0f, 0.0f };
		m_defaultShader = CEmbeddedShaders::Get().get_embedded_shader("Sprite");
		m_currentTextureUniform = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);


		m_backbufferShader = CEmbeddedShaders::Get().get_embedded_shader("Backbuffer");
		m_backbufferTextureUniform = bgfx::createUniform("s_backbuffer", bgfx::UniformType::Sampler);
		m_backbufferTexture = BGFX_INVALID_HANDLE;
		m_backbuffer = BGFX_INVALID_HANDLE;

		bgfx::ViewId order[] =
		{
			m_defaultView,
			m_backbufferView,
			m_uiView
		};

		bgfx::setViewOrder(m_defaultView, BX_COUNTOF(order), order);

		return true;
	}

	void CSceneRenderer::terminate()
	{
	}

	void CSceneRenderer::on_pre_render(ICamera* render_camera)
	{
		m_currentRenderCamera = render_camera;


		if (!bgfx::isValid(m_backbuffer))
		{
			m_backbuffer = bgfx::createFrameBuffer(render_camera->get_viewport_width(), render_camera->get_viewport_height(),
												   bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP);

			m_backbufferTexture = bgfx::getTexture(m_backbuffer);


			MINT_ASSERT(bgfx::isValid(m_backbuffer) == true, "Failed creating the Framebuffer!");
			MINT_ASSERT(bgfx::isValid(m_backbufferTexture) == true, "Failed creating the Framebuffer Texture!");
		}


		bgfx::setViewClear(m_backbufferView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_DISCARD_COLOR_0, render_camera->get_view_clear_color());

		bgfx::setViewRect(m_backbufferView, render_camera->get_viewport_x(), render_camera->get_viewport_y(), render_camera->get_viewport_width(), render_camera->get_viewport_height());

		bgfx::setViewTransform(m_backbufferView, glm::value_ptr(m_currentRenderCamera->get_view_matrix()), glm::value_ptr(m_currentRenderCamera->get_projection_matrix()));

		bgfx::setViewFrameBuffer(m_backbufferView, m_backbuffer);

		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA);

		_start_batch();
	}


	void CSceneRenderer::on_post_render()
	{
		_flush_batch();



		bgfx::setViewClear(m_defaultView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, m_currentRenderCamera->get_view_clear_color());

		bgfx::setViewRect(m_defaultView, 0, 0, m_currentRenderCamera->get_viewport_width(), m_currentRenderCamera->get_viewport_height());


		bgfx::setTexture(0, m_backbufferTextureUniform, m_backbufferTexture);

 		bgfx::setViewTransform(m_defaultView, glm::value_ptr(m_currentRenderCamera->get_view_matrix()), glm::value_ptr(m_currentRenderCamera->get_projection_matrix()));

		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA);

		_fullscreen_quad(m_currentRenderCamera->get_viewport_width(), m_currentRenderCamera->get_viewport_height());

		bgfx::submit(m_defaultView, m_backbufferShader);


		m_currentRenderCamera = nullptr;
	}


	void CSceneRenderer::on_render(mint::IScene* scene)
	{
		auto& registry = scene->get_registry();

		for(auto& entity: registry.get_registry_view< component::SSprite >())
		{
			const auto& sprite =	registry.get_component< component::SSprite >(entity);
			const auto& transform = registry.get_component< component::STransform >(entity);

			_render_sprite(CUCA::transform_get_world_transform_matrix(entity),
				sprite.m_color, sprite.m_rect, sprite.m_textureHandle,
 						  sprite.m_flipX, sprite.m_flipY);
		}
	}


	void CSceneRenderer::on_render(Vector< entt::entity >& entities)
	{
		auto& registry = MINT_ACTIVE_SCENE()->get_registry();

		for (auto& entity : entities)
		{
			const auto& sprite = registry.get_component< component::SSprite >(entity);
			const auto& transform = registry.get_component< component::STransform >(entity);


			_render_sprite(CUCA::transform_get_world_transform_matrix(entity),
						   sprite.m_color, sprite.m_rect, sprite.m_textureHandle,
						   sprite.m_flipX, sprite.m_flipY);
		}
	}


	void CSceneRenderer::on_resize(ICamera* render_camera)
	{
	}


	void CSceneRenderer::_start_batch()
	{
		m_quadBuffer.reset();
		m_currentTexture = BGFX_INVALID_HANDLE;
		m_currentTextureSize = { 0.0f, 0.0f };
	}


	void CSceneRenderer::_flush_batch()
	{
		// Set vertex + index buffer.
		m_quadBuffer.swap_buffers();

		auto vbh = m_quadBuffer.create_transient_vertex_buffer();
		auto ibh = m_quadBuffer.create_transient_index_buffer();


		bgfx::setVertexBuffer(0, vbh, 0, m_quadBuffer.get_vertices_count(), QuadVertex::s_VertexLayoutHandle);
		bgfx::setIndexBuffer(ibh, 0, m_quadBuffer.get_indices_count());


		bgfx::setTexture(0, m_currentTextureUniform, m_currentTexture);


		// Submit with shader to bgfx.
		bgfx::submit(m_backbufferView, m_defaultShader);
	}


	void CSceneRenderer::_next_batch()
	{
		_flush_batch(); _start_batch();
	}


	void CSceneRenderer::_render_sprite(const Mat4& transform, const CColor& color, const mint::CRect& rect, TextureHandle texture, bool flip_horizontal, bool flip_vertical)
	{
		if (!m_quadBuffer.has_room_for_another_quad() ||
			(m_currentTexture.idx != bgfx::kInvalidHandle && m_currentTexture.idx != texture.idx))
		{
			_next_batch();
		}

		m_currentTexture = texture;
		m_currentTextureSize = CTextureManager::Get().get_texture_dimension(texture);


		f32 u_modifier = 1.0f;
		f32 v_modifier = 1.0f;
		if (flip_horizontal) u_modifier = -1.0f;
		if (flip_vertical) v_modifier = -1.0f;

		Vec2 textureCoords[] =
		{
			{ u_modifier * rect.get_x(),					  v_modifier * (rect.get_y() + rect.get_height()) },	// Bottom left
			{ u_modifier * rect.get_x(),					  v_modifier * rect.get_y() },							// Top left
			{ u_modifier * (rect.get_x() + rect.get_width()), v_modifier * rect.get_y() },							// Top right
			{ u_modifier * (rect.get_x() + rect.get_width()), v_modifier * (rect.get_y() + rect.get_height()) },	// Bottom right
		};

		constexpr Vec4 quad_vertex_positions[4] =
		{
			{ -0.5f, -0.5f,		0.0f, 0.0f },	// Bottom left
			{ -0.5f, 0.5f,		0.0f, 0.0f },	// Top left
			{  0.5f, 0.5f,		0.0f, 0.0f },	// Top right
			{  0.5f,  -0.5f,	0.0f, 0.0f },	// Bottom right
		};

		QuadVertex quad_vertices[4];

		for (auto i = 0; i < 4; i++)
		{
			Vec4 vpos = transform * quad_vertex_positions[i];

			quad_vertices[i] = QuadVertex::make(vpos.x, vpos.y, color, textureCoords[i].x, textureCoords[i].y);
		}

		m_quadBuffer.add_quad(quad_vertices[0], quad_vertices[1], quad_vertices[2], quad_vertices[3]);
	}


	void CSceneRenderer::_fullscreen_quad(f32 texture_width, f32 texture_height)
	{
		bool origin_bottom_left = bgfx::getCaps()->originBottomLeft;

		f32 texel_half = bgfx::RendererType::Direct3D9 == bgfx::getCaps()->rendererType ? 0.5f : 0.0f;

		f32 _width = 1.0f;
		f32 _height = 1.0f;


		if (3 == bgfx::getAvailTransientVertexBuffer(3, ScreenQuadVertex::s_VertexLayout))
		{
			bgfx::TransientVertexBuffer vb;
			bgfx::allocTransientVertexBuffer(&vb, 3, ScreenQuadVertex::s_VertexLayout);
			auto vertex = (ScreenQuadVertex*)vb.data;

			const f32 zz = 0.0f;

			const f32 minx = -_width;
			const f32 maxx = _width;
			const f32 miny = 0.0f;
			const f32 maxy = _height * 2.0f;

			const f32 texelHalfW = texel_half / texture_width;
			const f32 texelHalfH = texel_half / texture_height;
			const f32 minu = -1.0f + texelHalfW;
			const f32 maxu = 1.0f + texelHalfW;

			f32 minv = texelHalfH;
			f32 maxv = 2.0f + texelHalfH;

			if (origin_bottom_left)
			{
				f32 temp = minv;
				minv = maxv;
				maxv = temp;

				minv -= 1.0f;
				maxv -= 1.0f;
			}

			vertex[0].m_x = minx;
			vertex[0].m_y = miny;
			vertex[0].m_z = zz;
			vertex[0].m_u = minu;
			vertex[0].m_v = minv;

			vertex[1].m_x = maxx;
			vertex[1].m_y = miny;
			vertex[1].m_z = zz;
			vertex[1].m_u = maxu;
			vertex[1].m_v = minv;

			vertex[2].m_x = maxx;
			vertex[2].m_y = maxy;
			vertex[2].m_z = zz;
			vertex[2].m_u = maxu;
			vertex[2].m_v = maxv;

			bgfx::setVertexBuffer(0, &vb);
 		}

// 		if (bgfx::getAvailTransientVertexBuffer(4, ScreenQuadVertex::s_VertexLayout))
// 		{
// 			bgfx::TransientVertexBuffer vb;
// 			bgfx::allocTransientVertexBuffer(&vb, 4, ScreenQuadVertex::s_VertexLayout);
// 			ScreenQuadVertex* vertex = (ScreenQuadVertex*)vb.data;
// 
// 			bgfx::TransientIndexBuffer ib;
// 			bgfx::allocTransientIndexBuffer(&ib, 6);
// 
// 			// 0
// 			vertex[0].m_x = -0.5f;
// 			vertex[0].m_y = 0.5f;
// 			vertex[0].m_z = 0.0f;
// 			vertex[0].m_u = 0.0f;
// 			vertex[0].m_v = 1.0f;
// 
// 			// 1
// 			vertex[1].m_x = 0.5f;
// 			vertex[1].m_y = -0.5f;
// 			vertex[1].m_z = 0.0f;
// 			vertex[1].m_u = 1.0f;
// 			vertex[1].m_v = 1.0f;
// 
// 			// 2
// 			vertex[2].m_x = 0.5f;
// 			vertex[2].m_y = 0.5f;
// 			vertex[2].m_z = 0.0f;
// 			vertex[2].m_u = 1.0f;
// 			vertex[2].m_v = 0.0f;
// 
// 			// 3
// 			vertex[3].m_x = -0.5f;
// 			vertex[3].m_y = 0.5f;
// 			vertex[3].m_z = 0.0f;
// 			vertex[3].m_u = 0.0f;
// 			vertex[3].m_v = 0.0f;
// 
// 
// 			u16* index = (u16*)ib.data;
// 			index[0] = 0;
// 			index[1] = 1;
// 			index[2] = 2;
//  			index[3] = 3;
// 			index[4] = 0;
// 			index[5] = 2;
// 
// 			bgfx::setVertexBuffer(0, &vb, 0, 6, ScreenQuadVertex::s_VertexLayoutHandle);
// 			bgfx::setIndexBuffer(&ib, 0, 6);
//  		}
	}

}