#include "LuaPhysicsModule.h"

namespace mint::luaglue
{
	namespace script
	{


		bool physics(lua_State* state)
		{
			bool initialization = true;

			initialization &= common(state);

			return initialization;
		}


	}

	namespace behavior
	{


		bool physics(lua_State* state)
		{
			bool initialization = true;

			initialization &= script::common(state);

			initialization &= rigid_body(state);

			return initialization;
		}


	}
}