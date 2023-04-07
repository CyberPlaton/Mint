#include "QuadBuffer.h"


namespace mint::fx
{

	void CQuadBuffer::add_quad(const QuadVertex& v1, const QuadVertex& v2, const QuadVertex& v3, const QuadVertex& v4)
	{
		auto vindex = _get_vertex_count();

		m_indices.push_back(SCAST(u16, vindex));
		m_indices.push_back(SCAST(u16, vindex) + SCAST(u16, 1));
		m_indices.push_back(SCAST(u16, vindex) + SCAST(u16, 2));
		
// 		m_indices.push_back(SCAST(u16, vindex));
// 		m_indices.push_back(SCAST(u16, vindex) + SCAST(u16, 2));
// 		m_indices.push_back(SCAST(u16, vindex) + SCAST(u16, 3));

		m_indices.push_back(SCAST(u16, vindex) + SCAST(u16, 3));
		m_indices.push_back(SCAST(u16, vindex));
		m_indices.push_back(SCAST(u16, vindex) + SCAST(u16, 2));


		m_vertices.push_back(v1);
		m_vertices.push_back(v2);
		m_vertices.push_back(v3);
		m_vertices.push_back(v4);
	}


	mint::u32 CQuadBuffer::_get_vertex_count()
	{
		return m_vertices.size();
	}


	bool CQuadBuffer::has_room_for_another_quad()
	{
		return (_get_vertex_count() + 4 < CQuadBuffer::s_MaxVertices && 
				get_vertices_count() + 4 < CQuadBuffer::s_MaxVertices);
	}


	bgfx::VertexBufferHandle CQuadBuffer::create_transient_vertex_buffer()
	{
		auto vcount = get_vertices_count();

		if(vcount <= bgfx::getAvailTransientVertexBuffer(vcount, QuadVertex::s_VertexLayout))
		{
			bgfx::allocTransientVertexBuffer(&m_tvb, vcount, QuadVertex::s_VertexLayout);

			auto mem_start = (QuadVertex*)m_tvb.data;

			bx::memCopy(mem_start, _get_vertices_begin(), vcount * sizeof(QuadVertex));

			return m_tvb.handle;
		}
	}


	bgfx::IndexBufferHandle CQuadBuffer::create_transient_index_buffer()
	{
		auto icount = get_indices_count();

		if (icount <= bgfx::getAvailTransientIndexBuffer(icount))
		{
			bgfx::allocTransientIndexBuffer(&m_tib, icount);

			auto mem_start = (u16*)m_tib.data;

			bx::memCopy(mem_start, _get_indices_begin(), icount * sizeof(u16));

			return m_tib.handle;
		}
	}


	void CQuadBuffer::reset()
	{
		m_vertices.clear();
		m_indices.clear();
		m_verticesFront.clear();
		m_indicesFront.clear();
	}


	CQuadBuffer::~CQuadBuffer()
	{
		reset();
	}


	void CQuadBuffer::swap_buffers()
	{
		// Push data from m_vertices/m_indices to m_verticesFront/m_indicesFront.
		std::swap(m_vertices, m_verticesFront);
		std::swap(m_indices, m_indicesFront);
	}


	mint::u32 CQuadBuffer::_get_index_count()
	{
		return m_indices.size();
	}


	void* CQuadBuffer::_get_vertices_begin()
	{
		return &m_verticesFront.front();
	}


	void* CQuadBuffer::_get_indices_begin()
	{
		return &m_indicesFront.front();
	}


	mint::u32 CQuadBuffer::get_vertices_count()
	{
		return m_verticesFront.size();
	}


	mint::u32 CQuadBuffer::get_indices_count()
	{
		return m_indicesFront.size();
	}


}