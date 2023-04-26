#ifndef _MINTSCRIPTING_SCRIPT_LUA_BINDING_SERVICE_IMPL_H_
#define _MINTSCRIPTING_SCRIPT_LUA_BINDING_SERVICE_IMPL_H_


#include "Utility/ServiceSystem/Services/ScriptLuaBindingService.h"
#include "../LuaBinding/ScriptingAllModules.h"

namespace mint::scripting
{

	class CScriptLuaBindingService : public IScriptLuaBindingService
	{
	public:
		bool initialize() override final;
		void terminate() override final;
		void reset() override final;

		bool register_script_functionality(lua_State* state) override final;

		bool register_behavior_functionality(lua_State* state) override final;
	};

}


#endif