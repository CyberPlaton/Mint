#ifndef _LUA_SCENE_MODULE_H_
#define _LUA_SCENE_MODULE_H_
#pragma once


#include "SceneLuaBinding.h"


namespace mint::luaglue
{
	namespace script
	{
		bool common(lua_State* state);
	}

	namespace behavior
	{
		bool common(lua_State* state);
	}
}

#endif