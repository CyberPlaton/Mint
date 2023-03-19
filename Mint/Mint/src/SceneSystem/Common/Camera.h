#ifndef _MINT_CAMERA_H_
#define _MINT_CAMERA_H_


#include "Common/Common.h"

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

		void recalculate_view(fx::SViewport& viewport);


		Vec4 get_world_visible_area();

		virtual Mat4 get_view_matrix();

		virtual Mat4 get_project_matrix();

		virtual Mat4 get_view_projection_matrix();


	private:
		Vec3 m_transform;

		f32 m_rotation;


		Mat4 m_projection;

		Mat4 m_view;

		Mat4 m_viewProjection;

	private:
		
	};
}


#endif