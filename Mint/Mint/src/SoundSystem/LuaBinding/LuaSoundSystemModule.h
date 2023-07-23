#ifndef _LUA_SOUND_SYSTEM_MODULE_H_
#define _LUA_SOUND_SYSTEM_MODULE_H_
#pragma once


#include "SoundSystemLuaBinding.h"


namespace mint::luaglue
{
	namespace script
	{
		bool luamodule_sound_system(lua_State* state);
	}

	namespace behavior
	{
		bool luamodule_sound_system(lua_State* state);
	}
}

#endif