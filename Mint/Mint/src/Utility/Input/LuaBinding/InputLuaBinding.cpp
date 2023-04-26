#include "InputLuaBinding.h"


namespace mint::luaglue
{

	namespace script
	{
		

		bool input(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("Input")
				.addFunction("key_pressed", &CInput::is_key_pressed_char)
				.addFunction("key_released", &CInput::is_key_released_char)
				.addFunction("key_held", &CInput::is_key_held_char)
				.addFunction("key_combination", &CInput::combination_char)

				.addFunction("mousebutton_pressed", &CInput::is_mouse_button_pressed)
				.addFunction("mousebutton_released", &CInput::is_mouse_button_released)
				.addFunction("mousebutton_held", &CInput::is_mouse_button_held)

				.addFunction("get_mouse_position", &CInput::get_mouse_position)
				.addFunction("set_mouse_position", &CInput::set_mouse_position)

				.addFunction("get_mouse_wheel", &CInput::get_mouse_wheel)

				.endNamespace();


			return true;
		}


	}
}

