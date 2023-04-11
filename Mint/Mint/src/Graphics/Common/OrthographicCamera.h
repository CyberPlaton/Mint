#ifndef _MINTFX_ORTHOGRAPHIC_CAMERA_H_
#define _MINTFX_ORTHOGRAPHIC_CAMERA_H_


#include "Camera.h"


namespace mint::fx
{

	class COrthographicCamera : public ICamera
	{
	public:
		COrthographicCamera(const SViewport& viewport);
		COrthographicCamera(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane);


		void update_view_matrix() override final;

		void update_projection_matrix() override final;


	public:
		f32 m_zoom;

	};
}



#endif