#include "SceneRenderer.h"


namespace mint::fx
{

	mint::fx::CSceneRenderer* CSceneRenderer::s_CSceneRenderer = nullptr;


	bool CSceneRenderer::initialize()
	{
		QuadVertex::initialize();
		ScreenQuadVertex::initialize();


#ifndef MINT_DISTR
 		bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif


		m_currentTexture = BGFX_INVALID_HANDLE;
		m_currentTextureSize = { 0.0f, 0.0f };
		m_defaultShader = CEmbeddedShaders::Get().get_embedded_shader("Sprite");
		m_currentTextureUniform = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

		m_backbufferTextureUniform = bgfx::createUniform("s_backbuffer", bgfx::UniformType::Sampler);

		m_backbufferOrthographicProjection = glm::ortho(-1, 1, -1, 1);

		return true;
	}

	void CSceneRenderer::terminate()
	{
		if (bgfx::isValid(m_backbuffer)) bgfx::destroy(m_backbuffer);
		if (bgfx::isValid(m_currentTextureUniform)) bgfx::destroy(m_currentTextureUniform);
		if (bgfx::isValid(m_backbufferTextureUniform)) bgfx::destroy(m_backbufferTextureUniform);
	}


	void CSceneRenderer::on_pre_render(ICamera* render_camera)
	{
		if(!bgfx::isValid(m_backbuffer))
		{
			m_backbuffer = bgfx::createFrameBuffer(render_camera->get_window_handle(), uint16_t(render_camera->get_viewport_width()), uint16_t(render_camera->get_viewport_height()), bgfx::TextureFormat::RGB8);
			m_backbufferTexture = bgfx::createTexture2D(uint16_t(render_camera->get_viewport_width()), uint16_t(render_camera->get_viewport_height()), false, 1, bgfx::TextureFormat::RGBA8, BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_U_CLAMP);
		}

		bgfx::setViewFrameBuffer(m_backbufferView, m_backbuffer);

		bgfx::touch(m_backbufferView);

		bgfx::setViewTransform(m_backbufferView, glm::value_ptr(render_camera->get_view_matrix()), glm::value_ptr(render_camera->get_projection_matrix()));

		bgfx::setViewRect(m_backbufferView, render_camera->get_viewport_x(), render_camera->get_viewport_y(), render_camera->get_viewport_width(), render_camera->get_viewport_height());

		bgfx::setState( render_camera->get_render_state() );
	}


	void CSceneRenderer::on_post_render(ICamera* render_camera)
	{
		bgfx::setViewFrameBuffer(m_backbufferView, BGFX_INVALID_HANDLE);


		bgfx::setViewTransform(m_defaultView, NULL, glm::value_ptr(m_backbufferOrthographicProjection));

		bgfx::setViewRect(m_defaultView, 0, 0, render_camera->get_viewport_width(), render_camera->get_viewport_height());

		bgfx::setTexture(m_defaultView, m_backbufferTextureUniform, m_backbufferTexture, NULL);

		bgfx::setState(m_defaultView, BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A );

		_fullscreen_quad(render_camera->get_viewport_width(), render_camera->get_viewport_height());

		bgfx::submit(m_defaultView, m_backbufferShader);

		bgfx::frame();

	}


	void CSceneRenderer::_fullscreen_quad(f32 texture_width, f32 texture_height)
	{
		const bgfx::RendererType::Enum renderer = bgfx::getRendererType();
		f32 _texelHalf = bgfx::RendererType::Direct3D9 == renderer ? 0.5f : 0.0f;
		bool _originBottomLeft = bgfx::RendererType::OpenGL == renderer || bgfx::RendererType::OpenGLES == renderer;
		f32 _width = 1.0f;
		f32 _height = 1.0f;

		if (bgfx::getAvailTransientVertexBuffer(3, ScreenQuadVertex::s_VertexLayout))
		{
			bgfx::TransientVertexBuffer vb;
			bgfx::allocTransientVertexBuffer(&vb, 3, ScreenQuadVertex::s_VertexLayout);
			ScreenQuadVertex* vertex = (ScreenQuadVertex*)vb.data;

			const f32 minx = -_width;
			const f32 maxx = _width;
			const f32 miny = 0.0f;
			const f32 maxy = _height * 2.0f;

			const f32 texelHalfW = _texelHalf / texture_width;
			const f32 texelHalfH = _texelHalf / texture_height;
			const f32 minu = -1.0f + texelHalfW;
			const f32 maxu = 1.0f + texelHalfH;

			float minv = texelHalfH;
			float maxv = 2.0f + texelHalfH;

			if (_originBottomLeft)
			{
				minv = 1.0f - minv;
				maxv = 1.0f - maxv;
			}

			vertex[0].m_x = minx;
			vertex[0].m_y = miny;
			vertex[0].m_z = 0.0f;
			vertex[0].m_u = minu;
			vertex[0].m_v = minv;

			vertex[1].m_x = maxx;
			vertex[1].m_y = miny;
			vertex[1].m_z = 0.0f;
			vertex[1].m_u = maxu;
			vertex[1].m_v = minv;

			vertex[2].m_x = maxx;
			vertex[2].m_y = maxy;
			vertex[2].m_z = 0.0f;
			vertex[2].m_u = maxu;
			vertex[2].m_v = maxv;

			bgfx::setVertexBuffer(0, &vb);
		}
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
		if (bgfx::isValid(m_backbuffer)) bgfx::destroy(m_backbuffer);
		if (bgfx::isValid(m_backbufferTexture)) bgfx::destroy(m_backbufferTexture);

		m_backbuffer = bgfx::createFrameBuffer(render_camera->get_window_handle(), uint16_t(render_camera->get_viewport_width()), uint16_t(render_camera->get_viewport_height()), bgfx::TextureFormat::RGB8);
		m_backbufferTexture = bgfx::createTexture2D(uint16_t(render_camera->get_viewport_width()), uint16_t(render_camera->get_viewport_height()), false, 1, bgfx::TextureFormat::RGBA8, BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_U_CLAMP);
	}


	void CSceneRenderer::_start_batch()
	{
		m_quadBuffer.reset();
		m_currentTexture = BGFX_INVALID_HANDLE;
		m_currentTextureSize = { 0.0f, 0.0f };
	}


	void CSceneRenderer::_flush_batch()
	{
		m_quadBuffer.swap_buffers();

		auto vbh = m_quadBuffer.create_transient_vertex_buffer();
		auto ibh = m_quadBuffer.create_transient_index_buffer();


		bgfx::setVertexBuffer(0, vbh, 0, m_quadBuffer.get_vertices_count(), QuadVertex::s_VertexLayoutHandle);
		bgfx::setIndexBuffer(ibh, 0, m_quadBuffer.get_indices_count());


		bgfx::setTexture(0, m_currentTextureUniform, m_currentTexture);

		bgfx::submit(m_backbufferView, m_defaultShader);
	}


	void CSceneRenderer::_next_batch()
	{
		_flush_batch(); _next_batch();
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

	bgfx::FrameBufferHandle CSceneRenderer::get_backbuffer()
	{
		return m_backbuffer;
	}


}