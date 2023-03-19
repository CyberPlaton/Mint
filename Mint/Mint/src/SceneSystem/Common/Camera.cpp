#include "Camera.h"


namespace mint
{


	CCamera::CCamera(fx::SViewport& main_scene_viewport)
	{
		m_projection = glm::ortho(main_scene_viewport.m_left, main_scene_viewport.m_right,
								  main_scene_viewport.m_bottom, main_scene_viewport.m_top, -1.0f, 1.0f);
	}


	CCamera::CCamera(f32 left, f32 right, f32 bottom, f32 top, f32 far, f32 near)
	{
		m_projection = glm::ortho(left, right, bottom, top, near, far);
	}


	CCamera::CCamera()
	{
	}


	void CCamera::recalculate_view()
	{
		Mat4 transform = glm::translate(glm::mat4(1.0f), m_transform) *
						 glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), Vec3(0, 0, 1));

		m_view = glm::inverse(transform);

		m_viewProjection = m_projection * m_view;
	}


	void CCamera::recalculate_view(fx::SViewport& viewport)
	{
		m_projection = glm::ortho(viewport.m_left, viewport.m_right,
								  viewport.m_bottom, viewport.m_top, -1.0f, 1.0f);

		recalculate_view();
	}


	mint::Vec4 CCamera::get_world_visible_area()
	{
		return { 0, 0, 0, 0 };
	}


	mint::Mat4 CCamera::get_view_matrix()
	{
		return m_view;
	}


	mint::Mat4 CCamera::get_project_matrix()
	{
		return m_projection;
	}


	mint::Mat4 CCamera::get_view_projection_matrix()
	{
		return m_viewProjection;
	}


}