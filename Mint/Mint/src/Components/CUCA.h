#ifndef _MINT_CUCA_H_
#define _MINT_CUCA_H_


#include "Components.h"

#include "SceneSystem/Common/IScene.h"
#include "Graphics/Shaders/MaterialManager.h"
#include "Scripting/BehaviorEngine.h"

namespace mint
{
	class IMintEngine;
}



namespace mint::component
{
	class CUCA
	{
		friend class IMintEngine;
	public:

		////////////////////////////////////////////////////////////////////////////////////////
		// Utility functions
		////////////////////////////////////////////////////////////////////////////////////////
		template< typename T >
		static bool entity_has_component(entt::entity entity);

		template < typename T >
		static T& entity_get_component(entt::entity entity);

		template< typename T >
		static T& entity_add_component(entt::entity entity);

		template< typename T >
		static void entity_remove_component(entt::entity entity);

		static Vec2 extract_position_from_world_transform(const Mat4& transform);
		static f32 extract_rotation_from_world_transform(const Mat4& transform);
		static Vec2 extract_scale_from_world_transform(const Mat4& transform);


		////////////////////////////////////////////////////////////////////////////////////////
		// Component functions
		////////////////////////////////////////////////////////////////////////////////////////

		static u64 animated_sprite_get_animation_speed(entt::entity entity);
		static void animated_sprite_set_animation_speed(entt::entity entity, u64 value);

		static mint::CRect sprite_get_destination_rect(entt::entity entity);
		static bool sprite_is_internal_visible(entt::entity entity);
		static void sprite_set_is_internal_visible(entt::entity entity, bool value);
		static void sprite_add_material(entt::entity entity, const fx::SMaterialDefinition& material_definition);
		static mint::CMap2< fx::CMaterial >& sprite_get_all_materials(entt::entity entity);
		static fx::CMaterial* sprite_get_main_material(entt::entity entity);
		static bool sprite_is_visible(entt::entity entity);
		static void sprite_set_is_visible(entt::entity entity, bool value);
		static u64 sprite_get_depth(entt::entity entity);
		static void sprite_set_depth(entt::entity entity, u64 value);
		static Vec2 sprite_get_size(entt::entity entity);
		static Vec2 sprite_get_origin(entt::entity entity);
		static void sprite_set_origin(entt::entity entity, const Vec2& value);
		static CRect sprite_get_source_rect(entt::entity entity);
		static void sprite_set_source_rect(entt::entity entity, const CRect& value);
		static bool sprite_is_flipped_x(entt::entity entity);
		static void sprite_set_is_flipped_x(entt::entity entity, bool value);
		static bool sprite_is_flipped_y(entt::entity entity);
		static void sprite_set_is_flipped_y(entt::entity entity, bool value);
		static fx::CColor sprite_get_color(entt::entity entity);
		static void sprite_set_color(entt::entity entity, const fx::CColor& value);

		static String identifier_get_debug_name(entt::entity entity);
		static void identifier_set_debug_name(entt::entity entity, const String& name);

		static u64 identifier_get_identifier(entt::entity entity);
		static u64 identifier_get_uuid(entt::entity entity);


		static bool hierarchy_has_parent(entt::entity entity);
		static bool hierarchy_has_children(entt::entity entity);
		static entt::entity hierarchy_get_parent(entt::entity entity);
		static Vector< entt::entity > hierarchy_get_children(entt::entity entity);
		static void hierarchy_set_parent(entt::entity entity, entt::entity parent);
		static void hierarchy_remove_parent(entt::entity entity);
		static void hierarchy_add_child(entt::entity entity, entt::entity child);
		static void hierarchy_remove_child(entt::entity entity, entt::entity child);

		/// @brief Transform operations applied to an entity that are hierarchical,
		/// meaning, if the entity has children we apply the same transformation operation to them too.
		/// @param entity 
		/// @param value 
		static void transform_scale(entt::entity entity, Vec2 value);
		static void transform_set_scale(entt::entity entity, Vec2 value);
		static void transform_translate(entt::entity entity, Vec2 value);
		static void transform_set_position(entt::entity entity, Vec2 value);
		static void transform_rotate(entt::entity entity, f32 value);
		static void transform_set_rotation(entt::entity entity, f32 value);

		static void transform_set_transform_matrix(entt::entity entity, const Mat4& matrix);
		
		static Vec2 transform_get_position(entt::entity entity);
		static f32 transform_get_rotation(entt::entity entity);
		static Vec2 transform_get_scale(entt::entity entity);



	private:
		static MINT_CRITICAL_SECTION(m_entityCriticalSection);
		static MINT_CRITICAL_SECTION(m_identifierCriticalSection);
		static MINT_CRITICAL_SECTION(m_hierarchyCriticalSection);
		static MINT_CRITICAL_SECTION(m_transformCriticalSection);
		static MINT_CRITICAL_SECTION(m_rigidBodyCriticalSection);
		static MINT_CRITICAL_SECTION(m_spriteCriticalSection);
		static MINT_CRITICAL_SECTION(m_animatedSpriteCriticalSection);
		static MINT_CRITICAL_SECTION(m_scriptCriticalSection);
		static MINT_CRITICAL_SECTION(m_dynamicGameobjectCriticalSection);


#ifndef MINT_DISTR
	public:
#else
	private:
#endif
		static Mat4 _get_world_transform(entt::entity entity);
		static Mat4 _get_local_transform(entt::entity entity);
		static void _update_world_transform_recursive(entt::entity entity);

		static void _rigid_body_update_translation(entt::entity entity, Vec2 value);
		static void _rigid_body_update_rotation(entt::entity entity, f32 value);
		static void _rigid_body_update_scale(entt::entity entity, Vec2 value);
	};


}


template< typename T >
bool mint::component::CUCA::entity_has_component(entt::entity entity)
{
	MINT_BEGIN_CRITICAL_SECTION(m_entityCriticalSection,

		auto& registry = MINT_ACTIVE_SCENE()->get_registry();

		const bool result = registry.has_component< T >(entity);
	);

	return result;
}

template < typename T >
T& mint::component::CUCA::entity_get_component(entt::entity entity)
{
	MINT_BEGIN_CRITICAL_SECTION(m_entityCriticalSection,

		auto& registry = MINT_ACTIVE_SCENE()->get_registry();

		auto& component = registry.get_component< T >(entity);
	);

	return component;
}

template< typename T >
T& mint::component::CUCA::entity_add_component(entt::entity entity)
{
	MINT_BEGIN_CRITICAL_SECTION(m_entityCriticalSection,

		auto& registry = MINT_ACTIVE_SCENE()->get_registry();

		auto& component = registry.add_component< T >(entity);
	);

	return component;
}


template< typename T >
void mint::component::CUCA::entity_remove_component(entt::entity entity)
{
	MINT_BEGIN_CRITICAL_SECTION(m_entityCriticalSection,

		auto& registry = MINT_ACTIVE_SCENE()->get_registry();

		registry.remove_component< T >(entity);
	);
}


using CUCA = mint::component::CUCA;


#endif