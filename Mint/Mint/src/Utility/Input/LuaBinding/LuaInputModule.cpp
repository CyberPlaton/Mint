#include "LuaInputModule.h"

namespace mint::luaglue
{
	namespace script
	{
		bool luamodule_input(lua_State* state)
		{
			bool initialization = true;

			initialization &= input(state);

			return initialization;
		}


	}

	namespace behavior
	{
		bool luamodule_input(lua_State* state)
		{
			bool initialization = true;

			initialization &= script::input(state);

			return initialization;
		}


	}
}