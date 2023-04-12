#ifndef _MINTFX_PERSPECTIVE_CAMERA_H_
#define _MINTFX_PERSPECTIVE_CAMERA_H_


#include "Camera.h"


namespace mint::fx
{

	class CPerspectiveCamera : public ICamera
	{
	public:
		CPerspectiveCamera(const SViewport& viewport);
		CPerspectiveCamera(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane);


		void update_view_matrix() override final;

		void update_projection_matrix() override final;


	public:
		f32 m_fov;

	};
}



#endif