#ifndef _LUA_COMPONENTS_MODULE_H_
#define _LUA_COMPONENTS_MODULE_H_
#pragma once

#include "ComponentsLuaBinding.h"

namespace mint::luaglue
{

	namespace script
	{
		bool luamodule_components(lua_State* state);
	}

	namespace behavior
	{
		bool luamodule_components(lua_State* state);
	}

}

#endif