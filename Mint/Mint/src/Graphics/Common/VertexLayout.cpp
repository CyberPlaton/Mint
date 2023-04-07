#include "VertexLayout.h"


namespace mint::fx
{
	bgfx::VertexLayout SPosColorTexcoordVertex::s_VertexLayout;
	bgfx::VertexLayoutHandle SPosColorTexcoordVertex::s_VertexLayoutHandle;

	bgfx::VertexLayout SPosTexcoordVertex::s_VertexLayout;
	bgfx::VertexLayoutHandle SPosTexcoordVertex::s_VertexLayoutHandle;


	void SPosColorTexcoordVertex::initialize()
	{
		s_VertexLayout.begin(bgfx::getRendererType())
			.add(bgfx::Attrib::Position,	3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,		4, bgfx::AttribType::Uint8, true)
			.add(bgfx::Attrib::TexCoord0,	2, bgfx::AttribType::Float)
			.end();

		s_VertexLayoutHandle = bgfx::createVertexLayout(s_VertexLayout);

		MINT_ASSERT(bgfx::isValid(s_VertexLayoutHandle) == true, "Failed creating Quad Vertex Layout!");
	}


	SPosColorTexcoordVertex SPosColorTexcoordVertex::make(f32 x, f32 y, CColor color, f32 u, f32 v)
	{
		return { x, y, 0.0f, color.as_abgr(), u, v };
	}



	void SPosTexcoordVertex::initialize()
	{
		s_VertexLayout.begin(bgfx::getRendererType())
			.add(bgfx::Attrib::Position,	3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0,	2, bgfx::AttribType::Float)
			.end();

		s_VertexLayoutHandle = bgfx::createVertexLayout(s_VertexLayout);

		MINT_ASSERT(bgfx::isValid(s_VertexLayoutHandle) == true, "Failed creating Screen Quad Vertex Layout!");
	}


	mint::fx::SPosTexcoordVertex SPosTexcoordVertex::make(f32 x, f32 y, f32 u, f32 v)
	{
		return { x, y, 0.0f, u, v };
	}


}