#include "SceneLuaBinding.h"


namespace mint::luaglue
{
	namespace script
	{


		bool camera(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginClass< mint::fx::ICamera >("Camera")

				.addFunction("set_translation", &mint::fx::ICamera::set_translation)
				.addFunction("set_translation_offset", &mint::fx::ICamera::set_translation_offset)
				.addFunction("set_rotation", &mint::fx::ICamera::set_rotation)
				.addFunction("set_zoom", &mint::fx::ICamera::set_zoom)

				.addFunction("get_world_visible_area", &mint::fx::ICamera::get_world_visible_area)

				.addFunction("get_viewport_x", &mint::fx::ICamera::get_viewport_x)
				.addFunction("get_viewport_y", &mint::fx::ICamera::get_viewport_y)
				.addFunction("get_viewport_width", &mint::fx::ICamera::get_viewport_width)
				.addFunction("get_viewport_height", &mint::fx::ICamera::get_viewport_height)

				.addFunction("get_clear_color", &mint::fx::ICamera::get_clear_color)

				.endClass();

			return true;
		}


		bool scene_common(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginClass< mint::IScene >("Scene")
				.addStaticFunction("get_active_scene", &IScene::get_active_scene)

				.addFunction("is_ready", &IScene::is_ready)
				.addFunction("get_scene_name", &IScene::get_scene_name)

				.addFunction("get_active_camera", &IScene::get_active_camera)
				.addFunction("push_camera", &IScene::push_camera)
				.addFunction("pop_camera", &IScene::pop_camera)

				.endClass();

			return true;
		}


	}

	namespace behavior
	{

	}

}