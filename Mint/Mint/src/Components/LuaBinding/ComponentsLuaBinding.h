#ifndef _COMPONENTS_LUA_BINDING_H_
#define _COMPONENTS_LUA_BINDING_H_
#pragma once


#include "Scripting/Common/Lua.h"

#include "../Components.h"
#include "../CUCA.h"


namespace mint::luaglue
{
	namespace script
	{
		
	}

	namespace behavior
	{
		bool identifier(lua_State* state);
	
		bool scene_hierarchy(lua_State* state);

		bool transform(lua_State* state);

		bool sprite(lua_State* state);

		bool animated_sprite(lua_State* state);
	}
}

#endif