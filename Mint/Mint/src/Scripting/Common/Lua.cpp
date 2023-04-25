#include "Lua.h"


namespace mint::scripting
{


	SError::SError(lua_State* state, s32 lua_error_value)
	{
		switch(lua_error_value)
		{
		case LUA_YIELD:
			m_value = "LUA_YIELD";
			break;
		case LUA_ERRRUN:
			m_value = "LUA_ERRRUN";
			break;
		case LUA_ERRSYNTAX:
			m_value = "LUA_ERRSYNTAX";
			break;
		case LUA_ERRMEM:
			m_value = "LUA_ERRMEM";
			break;
		case LUA_ERRGCMM:
			m_value = "LUA_ERRGCMM";
			break;
		case LUA_ERRERR:
			m_value = "LUA_ERRERR";
			break;
		default:
			m_value = "UNKNOWN";
			break;
		}

		m_message = lua_tostring(state, -1);
	}


}