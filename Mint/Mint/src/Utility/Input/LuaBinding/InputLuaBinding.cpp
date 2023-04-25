#include "InputLuaBinding.h"


namespace mint::luaglue
{

	namespace script
	{
		

		bool common(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("Input")
				.addFunction("KeyPressed", &CInput::KeyPressedC)
				.addFunction("KeyReleased", &CInput::KeyReleasedC)
				.addFunction("KeyHeld", &CInput::KeyHeldC)
				.addFunction("KeyCombination", &CInput::KeyCombination)

				.addFunction("MouseButtonPressed", &CInput::MouseButtonPressed)
				.addFunction("MouseButtonReleased", &CInput::MouseButtonReleased)
				.addFunction("MouseButtonHeld", &CInput::MouseButtonHeld)

				.addFunction("GetMousePosition", &CInput::GetMousePositionV)
				.addFunction("SetMousePosition", &CInput::SetMousePositionV)

				.addFunction("GetMouseWheel", &CInput::MouseWheel)

				.endNamespace();


			return true;
		}


	}
}

