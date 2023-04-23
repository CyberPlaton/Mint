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



	class ILuaScript
	{
	public:
		virtual bool initialize() = 0;

		virtual void terminate() = 0;

		virtual bool is_ready() const = 0;

		virtual bool has_error() const = 0;

		virtual bool has_entity_set() const = 0;


		virtual void on_update(f32) = 0;

		virtual void on_create() = 0;

		virtual void on_destroy() = 0;
	};



	class CLuaScript : public ILuaScript
	{
	public:
		CLuaScript();
		CLuaScript(const CLuaScript& other);
		CLuaScript& operator=(const CLuaScript& other);
		~CLuaScript();


		bool initialize();

		void terminate();

		bool is_ready() const { return m_ready; };

		bool has_error() const { return m_error; }

		bool has_entity_set() const { return m_entity != entt::null; };


		void on_update(f32 dt) {};

		void on_create() {};

		void on_destroy() {};


	protected:
		lua_State* m_state;

		bool m_ready;
		bool m_error;

		entt::entity m_entity;

		String m_scriptName;
		String m_scriptPath;
	};

}

#endif