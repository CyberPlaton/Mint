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
		ICamera(const CColor& clear_color = {0, 0, 0, 0});
		ICamera(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane, const CColor& clear_color = {0, 0, 0, 0});
		
		virtual void update_view_matrix() {};

		virtual void update_projection_matrix() {};

		virtual CRect get_world_visible_area() { CRect rect; return rect; }


		Mat4 get_view_matrix() { return m_view; }

		Mat4 get_projection_matrix() { return m_projection; }


		void set_translation(Vec3 value) { m_translation = value; update_view_matrix(); };

		void set_rotation(Vec3 value) { m_rotation = value; update_view_matrix(); };

		void set_scale(Vec3 value) { m_scale = value; update_view_matrix(); };


		f32 get_viewport_x() { return m_viewportRect.get_x(); }

		f32 get_viewport_y() { return m_viewportRect.get_y(); }

		f32 get_viewport_width() { return m_viewportRect.get_width(); }

		f32 get_viewport_height() { return m_viewportRect.get_height(); }

		f32 get_near_plane() { return m_nearFarPlanes.x; }

		f32 get_far_plane() { return m_nearFarPlanes.y; }

		void* get_window_handle() { return m_windowHandle; }


		virtual u64 get_render_state() { return m_renderState; }

		void set_render_state(u64 state);

		virtual u64 get_view_clear_state() { return m_viewClearState; }

		void set_view_clear_state(u64 state);

		virtual u32 get_view_clear_color() { return m_viewClearColor.as_rgba(); }

		CColor get_clear_color() { return m_viewClearColor; }

		void set_view_clear_color(u8 r, u8 g, u8 b, u8 a);


	public:
		Vec3 m_translation;

		Vec3 m_rotation;

		Vec3 m_scale;

		Mat4 m_projection;

		Mat4 m_view;



		CRect m_viewportRect;

		Vec2 m_nearFarPlanes;


		CColor m_viewClearColor;

		void* m_windowHandle;

		u64 m_renderState;

		u64 m_viewClearState;

	};


}


#endif