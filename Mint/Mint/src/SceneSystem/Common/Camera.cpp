#include "Camera.h"


namespace mint
{


	CCamera::CCamera(fx::SViewport& main_scene_viewport) :
		m_projection(Mat4(0.0f)), m_view(Mat4(0.0f))
	{
		m_viewport = { main_scene_viewport.m_left, main_scene_viewport.m_top, main_scene_viewport.m_right , main_scene_viewport.m_bottom };
	}


	CCamera::CCamera(f32 left, f32 right, f32 bottom, f32 top, f32 far, f32 near)
	{
		m_viewport = { left, top, right, bottom };
	}


	CCamera::CCamera() : 
		m_projection(1.0f), m_view(1.0f), m_viewProjection(1.0f),
		m_transform(0.0f), m_rotation(0.0f)
	{
	}


	void CCamera::recalculate_view_projection()
	{
		m_viewProjection = m_projection * m_view;
	}


	void CCamera::recalculate_view()
	{
		m_view = glm::lookAtLH(m_transform, m_lookAt, { 0.0f, 1.0f, 0.0f });

		recalculate_view_projection();
	}


	void CCamera::recalculate_projection()
	{
		m_projection = glm::perspectiveFovLH(mint::algorithm::degree_to_radians(m_fov), get_viewport_right(), get_viewport_bottom(), 0.01f, 1000.0f);

		recalculate_view_projection();
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


	mint::Vec4 CCamera::get_world_visible_area()
	{
		return { 0, 0, 0, 0 };
	}


	mint::Mat4 CCamera::get_view_matrix()
	{
		return m_view;
	}


	mint::Mat4 CCamera::get_inverse_view_matrix()
	{
		return glm::inverse(m_view);
	}


	mint::Mat4 CCamera::get_project_matrix()
	{
		return m_projection;
	}


	mint::Mat4 CCamera::get_view_projection_matrix()
	{
		return m_viewProjection;
	}


	mint::f32 CCamera::get_scale_x()
	{
		return 1.0f;
	}


	mint::f32 CCamera::get_scale_y()
	{
		return 1.0f;
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