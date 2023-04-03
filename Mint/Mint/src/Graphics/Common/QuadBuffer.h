#ifndef _MINTFX_QUAD_BUFFER_H_
#define _MINTFX_QUAD_BUFFER_H_


#include "Common/Common.h"
#include "Color.h"


namespace mint::fx
{
	
	struct SPosColorTexcoordVertex
	{
		static void initialize();

		static SPosColorTexcoordVertex make(f32 x, f32 y, CColor color, f32 u, f32 v);;


		static bgfx::VertexLayout s_VertexLayout;
		static bgfx::VertexLayoutHandle s_VertexLayoutHandle;

		SPosColorTexcoordVertex() = default;

		f32 m_x;
		f32 m_y;
		f32 m_z;
		
		u32 m_abgr;
		
		f32 m_u;
		f32 m_v;
	};


	using QuadVertex = SPosColorTexcoordVertex;

	class CSpriteBatch;

	class CQuadBuffer
	{
		friend class CSpriteBatch;

	public:
		static const u32 s_MaxVertices = 64535;

	public:
		CQuadBuffer() = default;
		~CQuadBuffer();

		void reset();

		void add_quad(const QuadVertex& v1, const QuadVertex& v2, const QuadVertex& v3, const QuadVertex& v4);
		
		bool has_room_for_another_quad();

		u32 get_vertices_count();

		u32 get_indices_count();

		void swap_buffers();

		bgfx::VertexBufferHandle create_transient_vertex_buffer();

		bgfx::IndexBufferHandle create_transient_index_buffer();

	private:
		/// @brief Vertex vector used for storing current submitted vertices.
		mint::Vector< QuadVertex > m_vertices;
		mint::Vector< u16 > m_indices;

		/// @brief Vertex vector used for submitting completed
		/// vertices to bgfx Vertex buffer.
		mint::Vector< QuadVertex > m_verticesFront;
		mint::Vector< u16 > m_indicesFront;

		bgfx::TransientVertexBuffer m_tvb;

		bgfx::TransientIndexBuffer m_tib;


	private:
		u32 _get_vertex_count();

		u32 _get_index_count();

		void* _get_vertices_begin();

		void* _get_indices_begin();
	};
}


#endif