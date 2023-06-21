#include "PrimitiveRenderer.h"


namespace mint::fx
{


	void CPrimitiveRenderer::render_rectangle_lines(const Vec2& position, const Vec2& dimension, const CColor& tint /*= MINT_WHITE()*/, f32 thikness /*= 1.0f*/)
	{
		DrawRectangleLinesEx({ position.x, position.y, dimension.x, dimension.y }, thikness, tint.as_cliteral());
	}

	void CPrimitiveRenderer::render_rectangle_lines(const CRect& rect, const CColor& tint /*= MINT_WHITE()*/, f32 thikness /*= 1.0f*/)
	{
		render_rectangle_lines({rect.get_x(), rect.get_y()}, {rect.get_width(), rect.get_height()}, tint, thikness);
	}


	void CPrimitiveRenderer::render_circle_outlined_lines(const Vec2& position, f32 radius, const CColor& tint /*= MINT_WHITE()*/, const CColor& outline_tint/*= MINT_BLACK()*/)
	{
		DrawCircle(position.x, position.y, radius + 1.0f, outline_tint.as_cliteral());
		DrawCircle(position.x, position.y, radius, tint.as_cliteral());
	}


	void CPrimitiveRenderer::render_line(const Vec2& a, const Vec2& b, const CColor& tint /*= MINT_WHITE()*/, f32 thikness /*= 1.0f*/)
	{
		DrawLineEx({ a.x, a.y }, { b.x, b.y }, thikness, tint.as_cliteral());
	}


	void CPrimitiveRenderer::render_text(const String& text, const Vec2& position, u32 size, const CColor& tint /*= MINT_WHITE()*/)
	{
		DrawText(text.c_str(), position.x, position.y, size, tint.as_cliteral());
	}


	void CPrimitiveRenderer::render_text_outlined(const String& text, const Vec2& position, u32 size, u32 outline_size /*= 3*/, const CColor& tint /*= MINT_WHITE()*/, const CColor& outline_tint /*= MINT_BLACK()*/)
	{
		render_text(text, position, size + outline_size, outline_tint);
		render_text(text, position, size, tint);
	}

	void CPrimitiveRenderer::render_rectangle_filled(const Vec2& position, const Vec2& dimension, const CColor& tint /*= MINT_WHITE()*/)
	{
		DrawRectangleV({ position.x, position.y }, { dimension.x, dimension.y }, tint.as_cliteral());
	}

	void CPrimitiveRenderer::render_rectangle_filled(const CRect& rect, const CColor& tint /*= MINT_WHITE()*/)
	{
		render_rectangle_filled({ rect.get_x(), rect.get_y() }, { rect.get_width(), rect.get_height() }, tint);
	}

	void CPrimitiveRenderer::render_aabb_lines(const b2AABB& aabb, const CColor& tint /*= MINT_WHITE()*/, f32 thikness /*= 1.0f*/, bool full_information /*= true*/)
	{
		auto center = aabb.GetCenter();
		auto extents = aabb.GetExtents();

		Vec2 tl = { center.x - extents.x, center.y - extents.y };
		Vec2 tr = { center.x + extents.x, center.y - extents.y };
		Vec2 br = { center.x + extents.x, center.y + extents.y };
		Vec2 bl = { center.x - extents.x, center.y + extents.y };

		// Render rectangle lines.
		render_line(tl, tr, tint, thikness);
		render_line(tr, br, tint, thikness);
		render_line(br, bl, tint, thikness);
		render_line(bl, tl, tint, thikness);

		// Render lines from center to bottom left and upper right.
		render_line({ center.x, center.y }, tr, tint, thikness);
		render_line({ center.x, center.y }, bl, tint, thikness);

		// Render center circle.
		render_circle_outlined_lines({ center.x, center.y }, 3.0f, tint, MINT_BLACK());

		if (full_information)
		{
			// Render position text of important values.
			u32 text_size = 1;
			render_text_outlined(TextFormat("{%.2f:%.2f}", bl.x, bl.y), { bl.x, bl.y + 5.0f }, text_size, text_size + 1, MINT_RED_DARK(), MINT_BLACK());
			render_text_outlined(TextFormat("{%.2f:%.2f}", center.x, center.y), { center.x, center.y + 5.0f }, text_size, text_size + 1, MINT_RED_DARK(), MINT_BLACK());
			render_text_outlined(TextFormat("{%.2f:%.2f}", tr.x, tr.y), { tr.x, tr.y + 5.0f }, text_size, text_size + 1, MINT_RED_DARK(), MINT_BLACK());

			// Render the lines for extents.
			render_line({ center.x, center.y }, { center.x + extents.x, center.y }, MINT_BLACK(), thikness);
			render_line({ center.x, center.y }, { center.x, center.y - extents.y }, MINT_BLACK(), thikness);

			// Render size values of extents as text.
			render_text_outlined(TextFormat("x=%.2f", extents.x), mint::algorithm::vec2_compute_center_between_two_vectors({ center.x, center.y }, { center.x + extents.x, center.y }), text_size, text_size + 1, MINT_RED_DARK(), MINT_BLACK());
			render_text_outlined(TextFormat("y=%.2f", extents.y), mint::algorithm::vec2_compute_center_between_two_vectors({ center.x, center.y }, { center.x, center.y - extents.y }), text_size, text_size + 1, MINT_RED_DARK(), MINT_BLACK());
		}
	}

	void CPrimitiveRenderer::render_aabb_filled(const b2AABB& aabb, const CColor& tint /*= MINT_WHITE()*/, bool full_information /*= true*/)
	{
		auto center = aabb.GetCenter();
		auto extents = aabb.GetExtents();

		Vec2 tl = { center.x - extents.x, center.y - extents.y };
		f32 w = extents.x * 2;
		f32 h = extents.y * 2;

		// Render filled rect.
		render_rectangle_filled(tl, { w, h }, tint);

		// Render aabb lines on top.
		render_aabb_lines(aabb, MINT_GREY(), 1.0f, full_information);
	}


}