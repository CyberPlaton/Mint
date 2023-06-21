#ifndef _MINTFX_PRIMITIVE_RENDERER_H_
#define _MINTFX_PRIMITIVE_RENDERER_H_


#include "Common/Common.h"
#include "../../Common/Color.h"
#include "Common/Rectangle.h"
#include "Common/Algorithm.h"


namespace mint::fx
{

	class CPrimitiveRenderer
	{
	public:
		static void render_aabb_lines(const b2AABB& aabb, const CColor& tint = MINT_WHITE(), f32 thikness = 1.0f, bool full_information = true);
		static void render_aabb_filled(const b2AABB& aabb, const CColor& tint = MINT_WHITE(), bool full_information = true);

		static void render_rectangle_filled(const Vec2& position, const Vec2& dimension, const CColor& tint = MINT_WHITE());
		static void render_rectangle_filled(const CRect& rect, const CColor& tint = MINT_WHITE());
		static void render_rectangle_lines(const Vec2& position, const Vec2& dimension, const CColor& tint = MINT_WHITE(), f32 thikness = 1.0f);
		static void render_rectangle_lines(const CRect& rect, const CColor& tint = MINT_WHITE(), f32 thikness = 1.0f);
		static void render_circle_outlined_lines(const Vec2& position, f32 radius, const CColor& tint = MINT_WHITE(), const CColor& outline_tint = MINT_BLACK());
		static void render_line(const Vec2& a, const Vec2& b, const CColor& tint = MINT_WHITE(), f32 thikness = 1.0f);
		static void render_text(const String& text, const Vec2& position, u32 size, const CColor& tint = MINT_WHITE());
		static void render_text_outlined(const String& text, const Vec2& position, u32 size, u32 outline_size = 3, const CColor& tint = MINT_WHITE(), const CColor& outline_tint = MINT_BLACK());
	};

}


#endif