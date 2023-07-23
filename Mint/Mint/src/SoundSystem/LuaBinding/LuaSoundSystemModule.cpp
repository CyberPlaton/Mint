#include "LuaSoundSystemModule.h"

namespace mint::luaglue
{
	namespace script
	{


		bool luamodule_sound_system(lua_State* state)
		{
			bool initialization = true;

			initialization &= sound_system_common(state);

			return initialization;
		}


	}

	namespace behavior
	{


		bool luamodule_sound_system(lua_State* state)
		{
			bool initialization = true;

			initialization &= script::sound_system_common(state);

			initialization &= sound_source(state);

			return initialization;
		}


	}
}