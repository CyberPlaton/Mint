#ifndef _LUA_COMMON_MODULE_H_
#define _LUA_COMMON_MODULE_H_
#pragma once


#include "CommonLuaBinding.h"


namespace mint::luaglue
{
	namespace script
	{
		bool luamodule_common(lua_State* state);
	}

	namespace behavior
	{
		bool luamodule_common(lua_State* state);
	}
}

#endif