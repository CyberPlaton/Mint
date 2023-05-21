#include "LuaScript.h"


namespace mint::scripting
{

	CLuaScript::CLuaScript() :
		m_state(nullptr), m_ready(false), m_error(false), m_active(false), m_entity(SCAST(EntityHandle, -1))
	{
	}


 	CLuaScript::CLuaScript(const CLuaScript& other) :
 		m_state(nullptr), m_ready(false), m_error(false), m_entity(SCAST(EntityHandle, -1))
 	{
 		m_scriptName = other.m_scriptName;
 		m_scriptPath = other.m_scriptPath;
 	}
 
 
 	CLuaScript& CLuaScript::operator=(const CLuaScript& other)
 	{
 		m_ready = false;
 		m_error = false;
 		m_active = false;
 		m_entity = SCAST(EntityHandle, -1);
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
		m_state = luaL_newstate();
		luaL_openlibs(m_state);


		auto reg = CServiceLocator::Get().locate_service< IScriptLuaBindingService >("LuaRegistration");

		MINT_ASSERT(reg != nullptr, "Lua script function binding service was not registered as a Service! ");

		bool init;

		if (has_entity_set())
		{
			MINT_LOG_INFO("[{:.4f}][CLuaScript::initialize] Initializing lua script \"{}\" for entity \"{}\".", MINT_APP_TIME, m_scriptName, SCAST(u64, m_entity));
			init = reg->register_behavior_functionality(m_state);
		}
		else
		{
			MINT_LOG_INFO("[{:.4f}][CLuaScript::initialize] Initializing lua script \"{}\" for active scene.", MINT_APP_TIME, m_scriptName);
			init = reg->register_script_functionality(m_state);
		}

		if (init && parse_check())
		{
			return true;
		}

		MINT_LOG_ERROR("[{:.4f}][CLuaScript::initialize] Failed initilaizing and/or registering script/behavior functionality!", MINT_APP_TIME);

		return false;
	}


	void CLuaScript::terminate()
	{
		set_active(false);
		m_ready = false;

		if(m_state) lua_close(m_state);
		m_state = nullptr;
		m_entity = SCAST(EntityHandle, -1);
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


	void CLuaScript::set_script_entity(EntityHandle entity)
	{
		// Please note, it is explicitly allowed for the handle to be invalid.
		m_entity = entity;
	}


	bool CLuaScript::parse_check()
	{
		s32 result;

		result = luaL_loadfilex(m_state, m_scriptPath.c_str(), "bt");

		if (result == LUA_OK) result = luaL_dofile(m_state, m_scriptPath.c_str());
		
		if (result == LUA_OK)
		{
			set_active(true);
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


	void CLuaScript::set_script_entity_verified(EntityHandle entity)
	{
		MINT_ASSERT(is_handle_valid(entity) == true, "Invalid operation. Entity for the script is invalid!");

		m_entity = entity;
	}


}