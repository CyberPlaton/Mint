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

		reg &= mint::luaglue::script::luamodule_common(state);
		reg &= mint::luaglue::script::luamodule_components(state);
		reg &= mint::luaglue::script::luamodule_scene(state);
		reg &= mint::luaglue::script::luamodule_input(state);
		reg &= mint::luaglue::script::luamodule_physics(state);

		return reg;
	}


	bool CScriptLuaBindingService::register_behavior_functionality(lua_State* state)
	{
		bool reg = true;

		reg &= mint::luaglue::behavior::luamodule_common(state);
		reg &= mint::luaglue::behavior::luamodule_components(state);
		reg &= mint::luaglue::behavior::luamodule_scene(state);
		reg &= mint::luaglue::behavior::luamodule_input(state);
		reg &= mint::luaglue::behavior::luamodule_physics(state);

		return reg;
	}


}