#include "LuaSceneModule.h"

namespace mint::luaglue
{
	namespace script
	{
		bool luamodule_scene(lua_State* state)
		{
			bool initialization = true;

			initialization &= camera(state);

			initialization &= scene_common(state);

			return initialization;
		}


	}

	namespace behavior
	{
		bool luamodule_scene(lua_State* state)
		{
			bool initialization = true;

			initialization &= script::camera(state);

			initialization &= script::scene_common(state);

			return initialization;
		}


	}
}