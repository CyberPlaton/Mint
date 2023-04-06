#include "SpriteBatch.h"



namespace mint::fx
{

	CSpriteBatch::CSpriteBatch()
	{
		m_previousTexture = BGFX_INVALID_HANDLE;
		m_spriteShader = CEmbeddedShaders::Get().get_embedded_shader("Sprite");

		m_spriteTextureHandle = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);
	}


	CSpriteBatch::~CSpriteBatch()
	{
		bgfx::destroy(m_spriteTextureHandle);
	}


	void CSpriteBatch::start_batch(bgfx::ViewId view)
	{
		m_view = view;
		m_qbuffer.reset();
		m_previousTexture = BGFX_INVALID_HANDLE;
	}


	void CSpriteBatch::flush_batch()
	{
		m_qbuffer.swap_buffers();

		auto vbh = m_qbuffer.create_transient_vertex_buffer();
		auto ibh = m_qbuffer.create_transient_index_buffer();

		
		bgfx::setVertexBuffer(0, vbh, 0, m_qbuffer.get_vertices_count(), QuadVertex::s_VertexLayoutHandle);
		bgfx::setIndexBuffer(ibh, 0, m_qbuffer.get_indices_count());


		bgfx::setTexture(0, m_spriteTextureHandle, m_previousTexture);


		bgfx::submit(m_view, m_spriteShader);
	}


	void CSpriteBatch::draw_sprite(const Vec2& position, f32 rotation, const Vec2& scale, const CColor& color, const mint::CRect& rect, TextureHandle texture, bool flipx, bool flipy)
	{
		if (!m_qbuffer.has_room_for_another_quad() ||
			(m_previousTexture.idx != bgfx::kInvalidHandle && m_previousTexture.idx != texture.idx))
		{
			next_batch();
		}

		// Store texture data.
		m_previousTexture = texture;
		m_previousTextureSize = CTextureManager::Get().get_texture_dimension(texture);

		Mat4 model = glm::translate(Mat4(1.0f), Vec3(position, 0.0f)) *

					glm::rotate(Mat4(1.0f), mint::algorithm::degree_to_radians(rotation), Vec3(0.0f, 0.0f, 1.0f)) *

 					 glm::scale(Mat4(1.0f), Vec3(scale, 1.0f));

 		draw_sprite(model, color, rect, texture, flipx, flipy);
	}


	void CSpriteBatch::draw_sprite(const Mat4& transform, const CColor& color, const mint::CRect& rect, TextureHandle texture, bool flipx, bool flipy)
	{
// 		const Vec2 textureCoords[] = 
// 		{ 
// 			{ 0.0f, 1.0f },		
// 			{ 1.0f, 1.0f },		
// 			{ 1.0f, 0.0f },		
// 			{ 0.0f, 0.0f }		
// 		};
		f32 u_modifier = 1.0f;
		f32 v_modifier = 1.0f;
		if (flipx) u_modifier = -1.0f;
		if (flipy) v_modifier = -1.0f;

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
		
		for(auto i = 0; i < 4; i++)
		{
			Vec4 vpos = transform * quad_vertex_positions[i];

 			quad_vertices[i] = QuadVertex::make(vpos.x, vpos.y, color, textureCoords[i].x, textureCoords[i].y);
		}

		m_qbuffer.add_quad(quad_vertices[0], quad_vertices[1], quad_vertices[2], quad_vertices[3]);
	}

}