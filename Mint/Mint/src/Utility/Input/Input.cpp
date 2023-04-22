#include "Input.h"


namespace mint
{
	mint::CInput* CInput::s_CInput = nullptr;

	bool CInput::combination_char(const String& keys)
	{
		for (auto& c : keys)
		{
			if (!is_key_held_char(c)) return false;
		}
		return true;
	}


	bool CInput::is_key_pressed_char(char c)
	{
		return is_key_pressed_enum(KeyboardKey(c));
	}


	bool CInput::is_key_released_char(char c)
	{
		return is_key_released_enum(KeyboardKey(c));
	}


	bool CInput::is_key_held_char(char c)
	{
		return is_key_held_enum(KeyboardKey(c));
	}


	bool CInput::is_key_pressed_enum(KeyboardKey k)
	{
		return IsKeyPressed(k);
	}


	bool CInput::is_key_released_enum(KeyboardKey k)
	{
		return IsKeyReleased(k);
	}


	bool CInput::is_key_held_enum(KeyboardKey k)
	{
		return IsKeyDown(k);
	}


	bool CInput::is_mouse_button_pressed(MouseButton mb)
	{
		return IsMouseButtonPressed(mb);
	}


	bool CInput::is_mouse_button_released(MouseButton mb)
	{
		return IsMouseButtonReleased(mb);
	}


	bool CInput::is_mouse_button_held(MouseButton mb)
	{
		return IsMouseButtonDown(mb);
	}


	f32 CInput::get_mouse_wheel()
	{
		return GetMouseWheelMove();
	}


	mint::Vec2 CInput::get_mouse_position()
	{
		return { GetMousePosition().x, GetMousePosition().y };
	}


	void CInput::set_mouse_position(const Vec2& v)
	{
		SetMousePosition((s32)v.x, (s32)v.y);
	}


}