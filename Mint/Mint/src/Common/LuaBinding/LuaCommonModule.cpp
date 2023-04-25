#include "LuaCommonModule.h"


namespace mint::luaglue
{
	namespace ubound
	{


		bool RegisterCommonUbound(lua_State* state)
		{
			bool initialization = true;

			initialization &= bound::CommonDependencies(state);

			return initialization;
		}


	}

	namespace bound
	{


		bool RegisterCommonBound(lua_State* state)
		{
			bool initialization = true;

			initialization &= CommonDependencies(state);

			return initialization;
		}


	}
}