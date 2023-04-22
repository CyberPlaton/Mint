#ifndef _MINT_INPUT_H_
#define _MINT_INPUT_H_


#include "Common/Common.h"


namespace mint
{

	class CInput
	{
	public:
		STATIC_GET(CInput, s_CInput);


		static bool combination_char(const String& keys);
		
		template < typename... ARGS >
		static bool combination_enum(const ARGS&... keys);

		static bool is_key_pressed_char(char c);
		static bool is_key_released_char(char c);
		static bool is_key_held_char(char c);

		static bool is_key_pressed_enum(KeyboardKey k);
		static bool is_key_released_enum(KeyboardKey k);
		static bool is_key_held_enum(KeyboardKey k);


		static bool is_mouse_button_pressed(MouseButton mb);
		static bool is_mouse_button_released(MouseButton mb);
		static bool is_mouse_button_held(MouseButton mb);

		static f32 get_mouse_wheel();
		static Vec2 get_mouse_position();
		static void set_mouse_position(const Vec2& v);

	private:
		static CInput* s_CInput;


	};


	template < typename... ARGS >
	bool mint::CInput::combination_enum(const ARGS&... keys)
	{
		auto va = { keys... };
		std::vector< KeyboardKey > vec{ va };

		for (auto& k : vec)
		{
			if (!is_key_held_enum(k)) return false;
		}
		return true;
	}


}


#endif