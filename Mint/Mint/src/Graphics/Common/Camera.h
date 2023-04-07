#ifndef _MINTFX_CAMERA_H_
#define _MINTFX_CAMERA_H_


#include "Common/Rectangle.h"
#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Graphics/Common/Viewport.h"


namespace mint::fx
{

	class ICamera
	{
	public:

		virtual void update_view_matrix() {};

		virtual void update_projection_matrix() {};


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


		virtual u64 get_render_state() { return BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL | BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA |
												BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA); }


	protected:
		Mat4 m_projection;

		Mat4 m_view;

		CRect m_viewportRect;

		Vec2 m_nearFarPlanes;


		Vec3 m_translation;

		Vec3 m_rotation;

		Vec3 m_scale;

		void* m_windowHandle;


		u64 m_renderState;
	};


}


#endif