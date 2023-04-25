#ifndef _MINTSCRIPTING_BEHAVIOR_H_
#define _MINTSCRIPTING_BEHAVIOR_H_


#include "../Common/LuaScript.h"

#include "Utility/ServiceSystem/Services/ScriptLuaBindingService.h"
#include "Utility/ServiceSystem/ServiceLocator.h"


namespace mint::scripting
{

	class CBehavior : public CLuaScript
	{
	public:
		void on_update(f32 dt = 0.0f) override final;

		void on_create() override final;

		void on_destroy() override final;

		void on_collision_enter();

		void on_collision_exit();


	};
}

#endif