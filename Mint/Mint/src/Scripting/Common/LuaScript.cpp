#include "LuaScript.h"


namespace mint::scripting
{

	CLuaScript::CLuaScript() :
		m_state(nullptr), m_ready(false), m_error(false), m_entity(entt::null)
	{
		m_state = luaL_newstate();

		luaL_openlibs(m_state);
	}


	CLuaScript::CLuaScript(const CLuaScript& other) :
		m_state(nullptr), m_ready(false), m_error(false), m_entity(entt::null)
	{
		m_state = luaL_newstate();

		luaL_openlibs(m_state);

		m_ready = other.m_ready;
		m_error = other.m_error;
		m_entity = other.m_entity;
		m_scriptName = other.m_scriptName;
		m_scriptPath = other.m_scriptPath;
	}


	CLuaScript& CLuaScript::operator=(const CLuaScript& other)
	{
		m_state = luaL_newstate();

		luaL_openlibs(m_state);

		m_ready = other.m_ready;
		m_error = other.m_error;
		m_entity = other.m_entity;
		m_scriptName = other.m_scriptName;
		m_scriptPath = other.m_scriptPath;

		return *this;
	}


	CLuaScript::~CLuaScript()
	{
		terminate();
	}


	bool CLuaScript::initialize()
	{
		auto reg = CServiceLocator::Get().locate_service< IScriptLuaBindingService >("LuaRegistration");

		MINT_ASSERT(reg != nullptr, "Lua script function binding service was not registered as a Service! ");

		bool init;

		if (has_entity_set())
		{
			init = reg->register_behavior_functionality(m_state);
		}
		else
		{
			init = reg->register_script_functionality(m_state);
		}

		if (init)
		{
			auto result = luaL_loadfilex(m_state, m_scriptPath.c_str(), "bt");

			if (result == LUA_OK)
			{
				luaL_dofile(m_state, m_scriptPath.c_str());

				m_ready = true;

				return true;
			}


			SError error(m_state, result);

			m_ready = false;
			m_error = true;

			lua_close(m_state);
			m_state = nullptr;

			MINT_LOG_ERROR("[{:.4f}][CLuaScript::initialize] Message: \"{}\", Errorvalue: \"{}\"", MINT_APP_TIME, error.m_message, error.m_value);
		}

		MINT_LOG_ERROR("[{:.4f}][CLuaScript::initialize] Failed registering script/behavior functionality!", MINT_APP_TIME);

		return false;
	}


	void CLuaScript::terminate()
	{
		lua_close(m_state);
		m_state = nullptr;
		m_ready = false;
		m_entity = entt::null;
		m_error = false;
	}


	mint::String CLuaScript::get_script_name() const
	{
		return m_scriptName;
	}


	mint::String CLuaScript::get_script_path() const
	{
		return m_scriptPath;
	}


	void CLuaScript::set_script_name(const String& name)
	{
		m_scriptName = name;
	}


	void CLuaScript::set_script_path(const String& path)
	{
		m_scriptPath = path;
	}


	void CLuaScript::set_script_entity(entt::entity entity)
	{
		m_entity = entity;
	}


}