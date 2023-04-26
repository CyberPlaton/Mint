#ifndef _LUA_INPUT_MODULE_H_
#define _LUA_INPUT_MODULE_H_
#pragma once


#include "InputLuaBinding.h"


namespace mint::luaglue
{
	namespace script
	{
		bool luamodule_input(lua_State* state);
	}

	namespace behavior
	{
		bool luamodule_input(lua_State* state);
	}
}

#endif