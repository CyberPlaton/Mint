#ifndef _SOUND_SYSTEM_LUA_BINDING_H_
#define _SOUND_SYSTEM_LUA_BINDING_H_
#pragma once


#include "Scripting/Common/Lua.h"

#include "../SoundSystem.h"


namespace mint::luaglue
{

	namespace script
	{
		bool sound_system_common(lua_State* state);
	}


	namespace behavior
	{
		bool sound_source(lua_State* state);
	}
}

#endif