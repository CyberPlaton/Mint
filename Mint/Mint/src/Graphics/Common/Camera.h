#ifndef _MINTFX_CAMERA_H_
#define _MINTFX_CAMERA_H_


#include "Common/Rectangle.h"
#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Common/Rectangle.h"
#include "Color.h"


namespace mint::fx
{

	class ICamera
	{
	public:
		ICamera(const CColor& clear_color = { 255, 255, 255, 255 }, const CRect & viewport = {0, 0, 1, 1});

		virtual CRect get_world_visible_area() { CRect rect; return rect; }

		virtual void use_camera() { MINT_ASSERT(false, "Invalid operation using the camera interface!"); }

		virtual void end_camera() { MINT_ASSERT(false, "Invalid operation using the camera interface!"); }

		
		virtual void set_translation(Vec2 value) { m_translation = value; };

		virtual void translate(Vec2 value) { m_translation += value; }

		virtual void set_translation_offset(Vec2 value)  { m_translationOffset = value; };

		virtual void translate_offset(Vec2 value) { m_translationOffset += value; }

		virtual void set_rotation(f32 value) { m_rotation = value; };

		virtual void rotate(f32 value) { m_rotation += value; }

		virtual void set_zoom(f32 value) { m_zoom = value; };

		virtual void zoom(f32 value) { m_zoom += value; }

		f32 get_viewport_x() { return m_viewportRect.get_x(); }

		f32 get_viewport_y() { return m_viewportRect.get_y(); }

		f32 get_viewport_width() { return m_viewportRect.get_width(); }

		f32 get_viewport_height() { return m_viewportRect.get_height(); }

		u32 get_view_clear_color() { return m_clearColor.as_rgba(); }

		CColor get_clear_color() { return m_clearColor; }

		void set_view_clear_color(u8 r, u8 g, u8 b, u8 a);


	public:
		Vec2 m_translation;

		Vec2 m_translationOffset;

		f32 m_rotation;

		f32 m_zoom;

		
		CRect m_viewportRect;

		CColor m_clearColor;
	};


}


#endif