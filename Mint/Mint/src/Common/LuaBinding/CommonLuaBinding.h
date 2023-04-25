#ifndef _COMMON_LUA_BINDING_H_
#define _COMMON_LUA_BINDING_H_
#pragma once


#include "Common/Common.h"

#include "Scripting/ScriptGlue.h"


namespace mint::luaglue
{

	namespace ubound
	{

	}


	namespace bound
	{
		////////////////////////////////////////////////////////////////////////////////////
		// Common Helpers
		////////////////////////////////////////////////////////////////////////////////////
		raylib::Vector2 Vector2DivideV(const raylib::Vector2& u, const raylib::Vector2& v);
		raylib::Vector2 Vector2MultiplyV(const raylib::Vector2& u, const raylib::Vector2& v);
		raylib::Vector2 Vector2AddV(const raylib::Vector2& u, const raylib::Vector2& v);
		raylib::Vector2 Vector2SubtractV(const raylib::Vector2& u, const raylib::Vector2& v);

		raylib::Vector2 Vector2DivideN(const raylib::Vector2& u, f32 v);
		raylib::Vector2 Vector2MultiplyN(const raylib::Vector2& u, f32 v);
		raylib::Vector2 Vector2AddN(const raylib::Vector2& u, f32 v);
		raylib::Vector2 Vector2SubtractN(const raylib::Vector2& u, f32 v);

		////////////////////////////////////////////////////////////////////////////////////
		// Registration
		////////////////////////////////////////////////////////////////////////////////////

		bool CommonDependencies(lua_State* state);
	}
}

#endif