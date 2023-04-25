#include "SceneLuaBinding.h"


namespace mint::luaglue
{
	namespace script
	{


		bool camera(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginClass< CCamera >("Camera")

				.addFunction("SetTarget", &CCamera::SetTarget)
				.addFunction("GetTarget", &CCamera::GetTarget)

				.addFunction("GetWorldTL", &CCamera::GetWorldTL)
				.addFunction("GetWorldBR", &CCamera::GetWorldBR)

				.addFunction("GetWorldVisibleArea", &CCamera::GetWorldVisibleArea)

				.addFunction("Zoom", &CCamera::Zoom)
				.addFunction("SetZoom", &CCamera::SetZoom)
				.addFunction("GetZoom", &CCamera::GetZoom)

				.addFunction("Rotate", &CCamera::Rotate)
				.addFunction("SetRotation", &CCamera::SetRotation)
				.addFunction("GetRotation", &CCamera::GetRotation)

				.endClass();

			return true;
		}


		bool scene_common(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginClass< IScene >("Scene")
				.addStaticFunction("GetActiveScene", &IScene::GetActiveScene)

				.addFunction("IsReady", &IScene::IsReady)
				.addFunction("GetSceneName", &IScene::GetSceneName)

				.addFunction("GetActiveCamera", &IScene::GetActiveCamera)
				.addFunction("PushCamera", &IScene::PushCamera)
				.addFunction("PopCamera", &IScene::PopCamera)

				.addFunction("RemoveEntity", &IScene::RemoveEntity)
				.addFunction("AddEntity", &IScene::AddEntity)
				.addFunction("GetEntities", &IScene::GetEntities)

				.endClass();

			return true;
		}


	}

	namespace behavior
	{

	}

}