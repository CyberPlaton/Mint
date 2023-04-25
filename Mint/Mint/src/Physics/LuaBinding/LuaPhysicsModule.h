#ifndef _LUA_PHYSICS_MODULE_H_
#define _LUA_PHYSICS_MODULE_H_
#pragma once


#include "PhysicsLuaBinding.h"


namespace mint::luaglue
{
	namespace script
	{
		bool physics(lua_State* state);
	}

	namespace behavior
	{
		bool physics(lua_State* state);
	}
}

#endif