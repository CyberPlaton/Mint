#include "PrimitiveRenderer.h"


namespace mint::fx
{


	void CPrimitiveRenderer::RenderRectangle(const Vec2& position, const Vec2& dimension, const CColor& tint /*= MINT_WHITE()*/)
	{
		RenderLine(position, { position.x + dimension.x, position.y}); // TL TR
		RenderLine({ position.x + dimension.x, position.y }, { position.x + dimension.x, position.y + dimension.y }); // TR BR
		RenderLine({ position.x + dimension.x, position.y + dimension.y }, { position.x, position.y + dimension.y }); // BR BL
		RenderLine({ position.x, position.y + dimension.y }, position); // BL TL
	}


	void CPrimitiveRenderer::RenderCircleOutlined(const Vec2& position, f32 radius, const CColor& tint /*= MINT_WHITE()*/, const CColor& outline_tint/*= MINT_BLACK()*/)
	{
		DrawCircle(position.x, position.y, radius + 1.0f, outline_tint.as_cliteral());
		DrawCircle(position.x, position.y, radius, tint.as_cliteral());
	}


	void CPrimitiveRenderer::RenderLine(const Vec2& a, const Vec2& b, const CColor& tint /*= MINT_WHITE()*/, f32 thikness /*= 1.0f*/)
	{
		DrawLineEx({ a.x, a.y }, { b.x, b.y }, thikness, tint.as_cliteral());
	}


	void CPrimitiveRenderer::RenderText(const String& text, const Vec2& position, u32 size, const CColor& tint /*= MINT_WHITE()*/)
	{
		DrawText(text.c_str(), position.x, position.y, size, tint.as_cliteral());
	}


	void CPrimitiveRenderer::RenderTextOutlined(const String& text, const Vec2& position, u32 size, const CColor& tint /*= MINT_WHITE()*/, const CColor& outline_tint /*= MINT_BLACK()*/)
	{
		RenderText(text, position, size + 2, outline_tint);
		RenderText(text, position, size, tint);
	}


}