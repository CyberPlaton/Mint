#ifndef _SCENE_LUA_BINDING_H_
#define _SCENE_LUA_BINDING_H_
#pragma once


#include "Scripting/Common/Lua.h"

#include "../Common/IScene.h"
#include "Graphics/Common/Camera.h"


namespace mint::luaglue
{

	namespace script
	{
		bool camera(lua_State* state);
	
		bool scene_common(lua_State* state);
	}

	namespace behavior
	{

	}

}

#endif