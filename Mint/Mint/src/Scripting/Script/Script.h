#ifndef _MINTSCRIPTING_SCRIPT_H_
#define _MINTSCRIPTING_SCRIPT_H_


#include "../Common/LuaScript.h"

#include "Utility/ServiceSystem/Services/ScriptLuaBindingService.h"
#include "Utility/ServiceSystem/ServiceLocator.h"


namespace mint::scripting
{

	class CScript : public CLuaScript
	{
	public:
		~CScript();

		void on_update(f32 dt = 0.0f) override final;

		void on_create() override final;

		void on_destroy() override final;

	};

}


#endif