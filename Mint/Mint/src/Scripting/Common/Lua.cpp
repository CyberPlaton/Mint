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
	}


	CLuaScript& CLuaScript::operator=(const CLuaScript& other)
	{
		m_state = luaL_newstate();

		luaL_openlibs(m_state);

		m_ready = other.m_ready;
		m_error = other.m_error;
		m_entity = other.m_entity;

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

		if(has_entity_set())
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


}