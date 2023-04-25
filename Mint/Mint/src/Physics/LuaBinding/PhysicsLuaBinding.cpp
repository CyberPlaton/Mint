#include "PhysicsLuaBinding.h"


namespace luabridge
{
	template <>
	struct luabridge::Stack <b2BodyType> : mint::scripting::SLuaBridgeEnumWrapper <b2BodyType>
	{
	};
}


namespace mint::luaglue
{
	namespace script
	{
		bool common(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginClass< SCollision >("Collision")
				.addData("Other", &SCollision::m_other, false)
				.endClass()

				.beginNamespace("Physics")

			.endNamespace();

			return true;
		}


	}

	namespace behavior
	{

		bool rigid_body(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("Physics")
				.addFunction("SetEnabled", &CUCA::SRigidBody_SetEnabled)
				.addFunction("GetEnabled", &CUCA::SRigidBody_GetEnabled)


				.addFunction("SetAllowSleep", &CUCA::SRigidBody_SetAllowSleep)
				.addFunction("GetAllowSleep", &CUCA::SRigidBody_GetAllowSleep)

				.addFunction("SetIsBullet", &CUCA::SRigidBody_SetIsBullet)
				.addFunction("GetIsBullet", &CUCA::SRigidBody_GetIsBullet)

				.addFunction("SetGravityScale", &CUCA::SRigidBody_SetGravityScale)
				.addFunction("GetGravityScale", &CUCA::SRigidBody_GetGravityScale)

				.addFunction("SetFixedRotation", &CUCA::SRigidBody_SetFixedRotation)
				.addFunction("GetFixedRotation", &CUCA::SRigidBody_GetFixedRotation)

				.addFunction("SetLinearDamping", &CUCA::SRigidBody_SetLinearDamping)
				.addFunction("GetLinearDamping", &CUCA::SRigidBody_GetLinearDamping)

				.addFunction("SetAngularDamping", &CUCA::SRigidBody_SetAngularDamping)
				.addFunction("GetAngularDamping", &CUCA::SRigidBody_GetAngularDamping)
				
				.addFunction("SetLinearVelocity", &CUCA::SRigidBody_SetLinearVelocity)
				.addFunction("SetAngularVelocity", &CUCA::SRigidBody_SetAngularVelocity)
				
				.addFunction("GetLinearVelocity", &CUCA::SRigidBody_GetLinearVelocity)
				.addFunction("GetAngularVelocity", &CUCA::SRigidBody_GetAngularVelocity)
				
				.addFunction("GetWorldMassCenter", &CUCA::SRigidBody_GetMassWorldCenter)
				.addFunction("GetLocaMassCenter", &CUCA::SRigidBody_GetMassLocalCenter)


				.endNamespace();

			return true;
		}


	}
}

