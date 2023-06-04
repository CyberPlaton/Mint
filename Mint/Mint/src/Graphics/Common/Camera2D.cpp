#include "Camera2D.h"


namespace mint::fx
{

	CCamera2D::CCamera2D(const CColor& clear_color, u32 x /*= 0*/, u32 y /*= 0*/, u32 w /*= 0*/, u32 h /*= 0*/, f32 z /*= 1.0f*/, f32 r /*= 0.0f*/) :
		ICamera(clear_color, {(f32)x, (f32)y, (f32)w, (f32)h})
	{
		set_translation({ x, y });
		set_translation_offset({ (f32)w / 2.0f, (f32)h / 2.0f });
		set_rotation(r);
		set_zoom(z);
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


	void CCamera2D::translate(Vec2 value)
	{
		ICamera::translate(value);

		auto vec = m_camera.GetTarget();

		m_camera.SetTarget({ value.x + vec.x, value.y + vec.y });
	}


	void CCamera2D::rotate(f32 value)
	{
		ICamera::rotate(value);

		auto r = m_camera.GetRotation();

		m_camera.SetRotation(r + value);
	}


	void CCamera2D::zoom(f32 value)
	{
		ICamera::zoom(value);

		auto z = m_camera.GetZoom();

		auto _zoom = (z + value <= 0.1f) ? 0.1f : z + value;

		m_camera.SetZoom(_zoom);
	}


	void CCamera2D::translate_offset(Vec2 value)
	{
		ICamera::translate_offset(value);

		auto vec = m_camera.GetOffset();

		m_camera.SetOffset({ vec.x + value.x, vec.y + value.y });
	}


	mint::Vec2 CCamera2D::get_position()
	{
		auto target = m_camera.GetTarget();

		return {target.x, target.y};
	}


	mint::Vec2 CCamera2D::get_position_offset()
	{
		auto offset = m_camera.GetOffset();

		return { offset.x, offset.y };
	}


	mint::f32 CCamera2D::get_rotation()
	{
		return m_camera.GetRotation();
	}


	mint::f32 CCamera2D::get_zoom()
	{
		return m_camera.GetZoom();
	}

	void CCamera2D::on_update(f32 dt)
	{
		for (auto effect : m_effects)
		{
			effect->on_update(this, dt);
		}
	}


}