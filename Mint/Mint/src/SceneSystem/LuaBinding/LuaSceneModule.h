#ifndef _LUA_SCENE_MODULE_H_
#define _LUA_SCENE_MODULE_H_
#pragma once


#include "SceneLuaBinding.h"


namespace mint::luaglue
{
	namespace script
	{
		bool luamodule_scene(lua_State* state);
	}

	namespace behavior
	{
		bool luamodule_scene(lua_State* state);
	}
}

#endif