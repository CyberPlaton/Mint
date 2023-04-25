#include "LuaInputModule.h"

namespace mint::luaglue
{
	namespace script
	{
		bool common(lua_State* state)
		{
			bool initialization = true;

			initialization &= common(state);

			return initialization;
		}


	}

	namespace behavior
	{
		bool common(lua_State* state)
		{
			bool initialization = true;

			initialization &= script::common(state);

			return initialization;
		}


	}
}