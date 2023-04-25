#ifndef _LUA_COMMON_MODULE_H_
#define _LUA_COMMON_MODULE_H_
#pragma once


#include "CommonLuaBinding.h"


namespace mint::luaglue
{
	namespace ubound
	{
		/// @brief Register all common functionality not bound to an Entity to a Lua state.
		/// @param state Lua state.
		/// @return True on success.
		bool RegisterCommonUbound(lua_State* state);
	}

	namespace bound
	{
		/// @brief Register all Entity bound common functionality to a Lua state.
		/// @param state Lua state.
		/// @return True on success.
		bool RegisterCommonBound(lua_State* state);
	}
}

#endif