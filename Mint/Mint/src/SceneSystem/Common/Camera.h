#ifndef _MINT_CAMERA_H_
#define _MINT_CAMERA_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Graphics/Common/Viewport.h"

#ifdef far
#undef far
#endif
#ifdef near
#undef near
#endif


namespace mint
{
	class CCamera
	{
	public:
		CCamera();
		CCamera(fx::SViewport& main_scene_viewport);
		CCamera(f32 left, f32 right, f32 bottom, f32 top, f32 far, f32 near);


		void recalculate_view();

		void recalculate_projection();



		void set_position(Vec3 position);

		void set_rotation(f32 angle);

		void set_zoom(f32 value);


		Vec4 get_world_visible_area();

		virtual Mat4 get_view_matrix();

		virtual Mat4 get_project_matrix();


		f32 get_viewport_left();

		f32 get_viewport_right();

		f32 get_viewport_bottom();

		f32 get_viewport_top();


	public:
		Vec4 m_viewport;

		Vec3 m_transform = Vec3(0.0f);

		Vec2 m_scale = Vec2(1.0f);

		f32 m_zoom = 1.0f;

		f32 m_rotation;

		Mat4 m_projection;

		Mat4 m_view;

	private:
		
	};
}


#endif