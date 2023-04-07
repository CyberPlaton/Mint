#ifndef _MINTFX_VERTEX_LAYOUT_H_
#define _MINTFX_VERTEX_LAYOUT_H_


#include "Common/Common.h"
#include "Color.h"


namespace mint::fx
{

	struct SPosColorTexcoordVertex
	{
		static void initialize();

		static SPosColorTexcoordVertex make(f32 x, f32 y, CColor color, f32 u, f32 v);


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



	struct SPosTexcoordVertex
	{
		static void initialize();

		static SPosTexcoordVertex make(f32 x, f32 y, f32 u, f32 v);

		SPosTexcoordVertex() = default;

		static bgfx::VertexLayout s_VertexLayout;
		static bgfx::VertexLayoutHandle s_VertexLayoutHandle;

		f32 m_x;
		f32 m_y;
		f32 m_z;

		f32 m_u;
		f32 m_v;
	};



	using QuadVertex = SPosColorTexcoordVertex;
	using ScreenQuadVertex = SPosTexcoordVertex;
}


#endif