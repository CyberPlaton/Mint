#include "FreeMovementCamera.h"


namespace mint::fx
{

	CFreeMovementCamera::CFreeMovementCamera(const CColor& clear_color, u32 x /*= 0*/, u32 y /*= 0*/, u32 w /*= 0*/, u32 h /*= 0*/, f32 z /*= 1.0f*/, f32 r /*= 0.0f*/) :
		CCamera2D(clear_color, x, y, w, h, z, r)
	{
	}

	void CFreeMovementCamera::on_update(f32 dt)
	{
		CCamera2D::on_update(dt);


		if (mint::CUI::ui_has_focus()) return;

		f32 speed = 1.0f;
		f32 zoom_speed = 1.0f;

		if (CInput::is_key_held_enum(KeyboardKey::KEY_LEFT_SHIFT))
		{
			speed = 0.5f;
			zoom_speed = 0.5f;
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_A))
		{
			translate({ -dt * speed, 0.0f });
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_D))
		{
			translate({ dt * speed, 0.0f });
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_W))
		{
			translate({ 0.0f, -dt * speed });
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_S))
		{
			translate({ 0.0f, dt * speed });
		}

		if (CInput::is_mouse_button_held(MOUSE_BUTTON_MIDDLE))
		{
			auto delta = GetMouseDelta();
			auto zoom = 1.0f / get_zoom();
			translate({ -delta.x * zoom, -delta.y * zoom });
		}

		if (f32 wheel_delta = CInput::get_mouse_wheel(); wheel_delta != 0.0f)
		{
			wheel_delta *= zoom_speed;

			zoom(wheel_delta);
		}
	}

}