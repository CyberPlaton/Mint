#ifndef _INPUT_LUA_BINDING_H_
#define _INPUT_LUA_BINDING_H_
#pragma once


#include "Scripting/Common/Lua.h"

#include "../Input.h"


namespace mint::luaglue
{

	namespace script
	{
		bool common(lua_State* state);
	}
}

#endif