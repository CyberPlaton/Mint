#include "Camera2D.h"


namespace mint::fx
{


	CCamera2D::CCamera2D(const CColor& clear_color)
	{

	}


	mint::CRect CCamera2D::get_world_visible_area()
	{
		auto tl = m_camera.GetScreenToWorld({ 0.0f, 0.0f });
		auto br = m_camera.GetScreenToWorld({ get_viewport_width(), get_viewport_height() });

		return { tl.x, tl.y, br.x, br.y };
	}


	void CCamera2D::use_camera()
	{
		BeginMode2D(m_camera);
	}


	void CCamera2D::end_camera()
	{
		EndMode2D();
	}


	void CCamera2D::set_translation(Vec2 value)
	{
		ICamera::set_translation(value);

		m_camera.SetTarget({ value.x, value.y });
	}


	void CCamera2D::set_translation_offset(Vec2 value)
	{
		ICamera::set_translation_offset(value);

		m_camera.SetOffset({ value.x, value.y });
	}


	void CCamera2D::set_rotation(f32 value)
	{
		ICamera::set_rotation(value);

		m_camera.SetRotation(value);
	}


	void CCamera2D::set_zoom(f32 value)
	{
		ICamera::set_zoom(value);

		m_camera.SetZoom(value);
	}


}