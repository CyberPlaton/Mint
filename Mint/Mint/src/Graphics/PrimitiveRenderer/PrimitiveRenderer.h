#ifndef _MINTFX_PRIMITIVE_RENDERER_H_
#define _MINTFX_PRIMITIVE_RENDERER_H_


#include "Common/Common.h"
#include "../Common/Color.h"


namespace mint::fx
{

	class CPrimitiveRenderer
	{
	public:
		static void RenderRectangle(const Vec2& position, const Vec2& dimension, const CColor& tint = MINT_WHITE(), f32 thikness = 1.0f);
		static void RenderCircleOutlined(const Vec2& position, f32 radius, const CColor& tint = MINT_WHITE(), const CColor& outline_tint = MINT_BLACK());
		static void RenderLine(const Vec2& a, const Vec2& b, const CColor& tint = MINT_WHITE(), f32 thikness = 1.0f);
		static void RenderText(const String& text, const Vec2& position, u32 size, const CColor& tint = MINT_WHITE());
		static void RenderTextOutlined(const String& text, const Vec2& position, u32 size, const CColor& tint = MINT_WHITE(), const CColor& outline_tint = MINT_BLACK());

	};

}


#endif