#ifndef _MINTSCRIPTING_LUA_SCRIPT_H_
#define _MINTSCRIPTING_LUA_SCRIPT_H_


#include "Lua.h"


namespace mint::scripting
{

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


		virtual String get_script_name() const = 0;

		virtual String get_script_path() const = 0;


		virtual void set_script_name(const String&) = 0;

		virtual void set_script_path(const String&) = 0;

		virtual void set_script_entity(entt::entity) = 0;

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

		String get_script_name() const;

		String get_script_path() const;

		void set_script_name(const String& name) override final;

		void set_script_path(const String& path)  override final;

		void set_script_entity(entt::entity entity) override final;


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