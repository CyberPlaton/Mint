#include "OrthographicCamera.h"


namespace mint::fx
{


	COrthographicCamera::COrthographicCamera(const SViewport& viewport) : 
		ICamera(viewport), m_zoom(1.0f)
	{
		update_projection_matrix();
	}


	COrthographicCamera::COrthographicCamera(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane) :
		ICamera(x, y, width, height, near_plane, far_plane), m_zoom(1.0f)
	{
		update_projection_matrix();
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