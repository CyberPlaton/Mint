#include "PerspectiveCamera.h"


namespace mint::fx
{


	CPerspectiveCamera::CPerspectiveCamera(const SViewport& viewport) : 
		ICamera(viewport), m_fov(45.0)
	{
		update_projection_matrix();
	}


	CPerspectiveCamera::CPerspectiveCamera(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane) :
		ICamera(x, y, width, height, near_plane, far_plane), m_fov(45.0f)
	{
		update_projection_matrix();
	}


	void CPerspectiveCamera::update_view_matrix()
	{
		m_view = glm::lookAt(m_translation, {0.0f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f });
	}


	void CPerspectiveCamera::update_projection_matrix()
	{
		m_projection = glm::perspective(mint::algorithm::degree_to_radians(m_fov), get_viewport_width() / get_viewport_height(), 0.01f, 1000.0f);
	}


}