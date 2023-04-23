#include "ScriptLuaBindingServiceImpl.h"


namespace mint::scripting
{


	bool CScriptLuaBindingService::initialize()
	{
		return true;
	}


	void CScriptLuaBindingService::terminate()
	{

	}


	void CScriptLuaBindingService::reset()
	{

	}


	bool CScriptLuaBindingService::register_script_functionality(lua_State* state)
	{
		bool reg = true;



		return reg;
	}


	bool CScriptLuaBindingService::register_behavior_functionality(lua_State* state)
	{
		bool reg = true;



		return reg;
	}


}