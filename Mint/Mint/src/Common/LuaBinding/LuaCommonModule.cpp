#include "LuaCommonModule.h"


namespace mint::luaglue
{
	namespace script
	{


		bool luamodule_common(lua_State* state)
		{
			bool initialization = true;

			initialization &= behavior::dependencies(state);

			return initialization;
		}


	}

	namespace behavior
	{

		bool luamodule_common(lua_State* state)
		{
			bool initialization = true;

			initialization &= dependencies(state);

			return initialization;
		}


	}
}