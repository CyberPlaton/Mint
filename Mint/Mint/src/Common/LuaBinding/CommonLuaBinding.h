#ifndef _COMMON_LUA_BINDING_H_
#define _COMMON_LUA_BINDING_H_
#pragma once


#include "Common/Common.h"

#include "Scripting/Common/Lua.h"


namespace mint::luaglue
{

	namespace script
	{

	}


	namespace behavior
	{
		Vec2 Vector2DivideV(const Vec2& u, const Vec2& v);
		Vec2 Vector2MultiplyV(const Vec2& u, const Vec2& v);
		Vec2 Vector2AddV(const Vec2& u, const Vec2& v);
		Vec2 Vector2SubtractV(const Vec2& u, const Vec2& v);

		Vec2 Vector2DivideN(const Vec2& u, f32 v);
		Vec2 Vector2MultiplyN(const Vec2& u, f32 v);
		Vec2 Vector2AddN(const Vec2& u, f32 v);
		Vec2 Vector2SubtractN(const Vec2& u, f32 v);

		
		bool dependencies(lua_State* state);
	}
}

#endif