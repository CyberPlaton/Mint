#include "OrthographicCamera.h"


namespace mint::fx
{


	COrthographicCamera::COrthographicCamera(const SViewport& viewport) : 
		m_zoom(1.0f)
	{
		m_viewportRect.set_rectangle(viewport.m_left, viewport.m_top, viewport.m_right, viewport.m_bottom);
		m_nearFarPlanes = { viewport.m_nearPlane, viewport.m_farPlane };

		m_view = Mat4(1.0f);
		m_projection = Mat4(1.0f);
		m_translation = Vec3(0.0f);
		m_rotation = Vec3(0.0f);
		m_scale = Vec3(0.0f);

		m_windowHandle = viewport.m_windowHandle;
	}


	COrthographicCamera::COrthographicCamera(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane) :
		m_zoom(1.0f)
	{
		m_viewportRect.set_rectangle(x, y, width, height);
		m_nearFarPlanes = { near_plane, far_plane };

		m_view = Mat4(1.0f);
		m_projection = Mat4(1.0f);
		m_translation = Vec3(0.0f);
		m_rotation = Vec3(0.0f);
		m_scale = Vec3(0.0f);
		m_windowHandle = nullptr;
	}


	void COrthographicCamera::update_view_matrix()
	{
		m_view = glm::translate(Mat4(1.0f), -Vec3(m_translation)) *

				 glm::rotate(Mat4(1.0f), mint::algorithm::degree_to_radians(m_rotation.z), Vec3(0.0f, 0.0f, 1.0f)) *
				
				 glm::scale(Mat4(1.0f), Vec3(Vec2(m_scale), 1.0f));
	}


	void COrthographicCamera::update_projection_matrix()
	{
		m_projection = glm::ortho(-m_zoom, m_zoom, -m_zoom, m_zoom);
	}


}