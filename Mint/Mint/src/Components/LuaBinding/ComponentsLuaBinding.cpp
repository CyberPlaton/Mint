#include "ComponentsLuaBinding.h"

namespace mint::luaglue
{
	namespace script
	{

	}

	namespace behavior
	{
		
		bool identifier(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginClass< entt::entity >("Entity")
				.endClass();

			luabridge::getGlobalNamespace(state)
				.beginNamespace("entity")

#ifndef MINT_DISTR
					.addFunction("get_debug_name", &CUCA::identifier_get_debug_name)				
					.addFunction("set_debug_name", &CUCA::identifier_set_debug_name)
#endif
					.addFunction("get_uuid", &CUCA::identifier_get_uuid)
					.addFunction("get_identifier", &CUCA::identifier_get_identifier)
 				.endNamespace();
			
			return true;
		}


		bool scene_hierarchy(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("entity")
					.addFunction("has_parent", &CUCA::hierarchy_has_parent)
					.addFunction("has_children", &CUCA::hierarchy_has_children)
					.addFunction("get_parent", &CUCA::hierarchy_get_parent)
					.addFunction("set_parent", &CUCA::hierarchy_set_parent)
					.addFunction("get_children", &CUCA::hierarchy_get_children)
					.addFunction("add_child", &CUCA::hierarchy_add_child)
					.addFunction("remove_child", &CUCA::hierarchy_remove_child)
				.endNamespace();

			return true;
		}


		bool transform(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("entity")
				
				.addFunction("get_position", &CUCA::transform_get_position)
				.addFunction("get_rotation", &CUCA::transform_get_rotation)
				.addFunction("get_scale", &CUCA::transform_get_scale)

				.addFunction("set_position_local", &CUCA::transform_set_position_local)
				.addFunction("set_rotation_local", &CUCA::transform_set_rotation_local)
				.addFunction("set_scale_local", &CUCA::transform_set_scale_local)

				.addFunction("set_position", &CUCA::transform_set_position)
				.addFunction("set_rotation", &CUCA::transform_set_rotation)
				.addFunction("set_scale", &CUCA::transform_set_scale)

				.addFunction("translate_local", &CUCA::transform_translate_local)
				.addFunction("rotate_local", &CUCA::transform_rotate_local)
				.addFunction("scale_local", &CUCA::transform_scale_local)

				.addFunction("translate", &CUCA::transform_translate)
				.addFunction("rotate", &CUCA::transform_rotate)
				.addFunction("scale", &CUCA::transform_scale)

				
				.endNamespace();

			return true;
		}


		bool sprite(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("entity")
				.addFunction("is_visible", &CUCA::sprite_is_visible)
				.addFunction("set_is_visible", &CUCA::sprite_set_is_visible)
				.addFunction("__is_internal_visible__", &CUCA::sprite_is_internal_visible)

				.addFunction("get_depth", &CUCA::sprite_get_depth)
				.addFunction("set_depth", &CUCA::sprite_set_depth)

				.addFunction("get_sprite_size", &CUCA::sprite_get_size)
				
				.addFunction("get_sprite_origin", &CUCA::sprite_get_origin)
				.addFunction("set_sprite_origin", &CUCA::sprite_set_origin)

				.addFunction("get_sprite_color", &CUCA::sprite_get_color)
				.addFunction("set_sprite_color", &CUCA::sprite_set_color)

				.addFunction("get_source_rect", &CUCA::sprite_get_source_rect)
				.addFunction("set_source_rect", &CUCA::sprite_set_source_rect)

				.addFunction("is_flipped_x", &CUCA::sprite_is_flipped_x)
				.addFunction("is_flipped_y", &CUCA::sprite_is_flipped_y)
				.addFunction("set_is_flipped_x", &CUCA::sprite_set_is_flipped_x)
				.addFunction("set_is_flipped_y", &CUCA::sprite_set_is_flipped_y)

				.endNamespace();

			return true;
		}


		bool animated_sprite(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("entity")

				.addFunction("get_animation_speed", &CUCA::animated_sprite_get_animation_speed)
				.addFunction("set_animation_speed", &CUCA::animated_sprite_set_animation_speed)

				.endNamespace();

			return true;
		}


	}
}