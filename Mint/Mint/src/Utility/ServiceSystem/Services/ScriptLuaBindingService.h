#ifndef _MINT_SCRIPT_LUABINDING_SERVICE_H_
#define _MINT_SCRIPT_LUABINDING_SERVICE_H_


#include "../Service.h"
#include "LuaBridge/LuaLibrary.h"


namespace mint
{

	class IScriptLuaBindingService : public IService
	{
	public:
		virtual bool initialize() { return true; };
		virtual void terminate()  {};
		virtual void reset()	  {};
		String get_service_type() { return "LuaRegistration"; }


		virtual bool register_script_functionality(lua_State* state) = 0;

		virtual bool register_behavior_functionality(lua_State* state) = 0;
	};


}


#endif