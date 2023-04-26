#include "LuaPhysicsModule.h"

namespace mint::luaglue
{
	namespace script
	{


		bool luamodule_physics(lua_State* state)
		{
			bool initialization = true;

			initialization &= physics_common(state);

			return initialization;
		}


	}

	namespace behavior
	{


		bool luamodule_physics(lua_State* state)
		{
			bool initialization = true;

			initialization &= script::physics_common(state);

			initialization &= rigid_body(state);

			return initialization;
		}


	}
}