#include "Camera.h"


namespace mint
{


	CCamera::CCamera(fx::SViewport& main_scene_viewport) :
		m_projection(Mat4(1.0f)), m_view(Mat4(0.0f))
	{
		m_viewport = { main_scene_viewport.m_left, main_scene_viewport.m_top, main_scene_viewport.m_right , main_scene_viewport.m_bottom };
	}


	CCamera::CCamera(f32 left, f32 right, f32 bottom, f32 top, f32 far, f32 near)
	{
		m_viewport = { left, top, right, bottom };
	}


	CCamera::CCamera() : 
		m_projection(1.0f), m_view(0.0f),
		m_transform(0.0f), m_rotation(0.0f)
	{
	}

	void CCamera::recalculate_view()
	{
		m_view = glm::translate(Mat4(1.0f), -Vec3(m_transform)) *

				 glm::rotate(Mat4(1.0f), mint::algorithm::degree_to_radians(m_rotation), Vec3(0.0f, 0.0f, 1.0f)) *

				 glm::scale(Mat4(1.0f), Vec3(m_scale, 1.0f));
	}


	void CCamera::recalculate_projection()
	{
		m_projection = glm::ortho(-m_zoom, m_zoom, -m_zoom, m_zoom);
	}


	void CCamera::set_position(Vec3 position)
	{
		m_transform = position;

		recalculate_view();
	}


	void CCamera::set_rotation(f32 angle)
	{
		m_rotation = angle;

		recalculate_view();
	}


	void CCamera::set_zoom(f32 value)
	{
		m_zoom = -value;

		recalculate_projection();
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


	mint::f32 CCamera::get_viewport_left()
	{
		return m_viewport.x;
	}


	mint::f32 CCamera::get_viewport_right()
	{
		return m_viewport.z;
	}


	mint::f32 CCamera::get_viewport_bottom()
	{
		return m_viewport.w;
	}


	mint::f32 CCamera::get_viewport_top()
	{
		return m_viewport.y;
	}


}