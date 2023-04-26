#ifndef _PHYSICS_LUA_BINDING_H_
#define _PHYSICS_LUA_BINDING_H_
#pragma once


#include "Scripting/Common/Lua.h"

#include "../Common/Collision.h"
#include "../PhysicsSystem.h"


namespace mint::luaglue
{

	namespace script
	{
		bool physics_common(lua_State* state);
	}


	namespace behavior
	{
		bool rigid_body(lua_State* state);
	}
}

#endif