#ifndef _MINTSCRIPTING_LUA_H_
#define _MINTSCRIPTING_LUA_H_


#include "Common/Common.h"

#include "LuaBridge/LuaLibrary.h"
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/List.h"
#include "LuaBridge/Vector.h"
#include "LuaBridge/Map.h"
#include "LuaBridge/Array.h"
#include "LuaBridge/UnorderedMap.h"
#include "LuaBridge/RefCountedObject.h"

#include "Utility/ServiceSystem/Services/ScriptLuaBindingService.h"
#include "Utility/ServiceSystem/ServiceLocator.h"


namespace mint::scripting
{

	struct SError
	{
		SError(lua_State* state, s32 lua_error_value);
		
		String m_value;
		String m_message;
	};


	template< class Vector >
	struct SLuaBridgeVectorWrapper
	{
		template< unsigned int index >
		static f32 get(Vector const* vec)
		{
			switch (index)
			{
			case 0:
				return vec->x;
			case 1:
				return vec->y;
			}
		}

		template< unsigned int index >
		static void set(Vector* vec, f32 value)
		{
			switch (index)
			{
			case 0:
			{
				vec->x = value;
				return;
			}
			case 1:
			{
				vec->y = value;
				return;
			}
			}
		}

	};


	template <typename T>
	struct SLuaBridgeEnumWrapper
	{
		static typename std::enable_if<std::is_enum<T>::value, void>::type push(lua_State* L, T value)
		{
			lua_pushnumber(L, static_cast<std::size_t> (value));
		}

		static typename std::enable_if<std::is_enum<T>::value, T>::type get(lua_State* L, int index)
		{
			return static_cast <T> (lua_tointeger(L, index));
		}
	};

}

#endif