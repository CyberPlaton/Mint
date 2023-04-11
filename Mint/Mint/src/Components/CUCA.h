#ifndef _MINT_CUCA_H_
#define _MINT_CUCA_H_


#include "Components.h"

#include "SceneSystem/Common/IScene.h"


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

		static Mat4 transform_get_world_transform_matrix(entt::entity entity);
		static Mat4 transform_get_local_transform_matrix(entt::entity entity);
		static Vec2 transform_get_position(entt::entity entity);
		static f32 transform_get_rotation(entt::entity entity);
		static Vec2 transform_get_scale(entt::entity entity);


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


		/// @brief Transform operations applied to an entity ignoring the entity hierarchy,
		/// meaning, if the entity has children, the transformation operation is not applied to them.
		/// @param entity 
		/// @param value 
		static void transform_scale_local(entt::entity entity, Vec2 value);
		static void transform_set_scale_local(entt::entity entity, Vec2 value);
		static void transform_set_position_local(entt::entity entity, Vec2 value);
		static void transform_translate_local(entt::entity entity, Vec2 value);
		static void transform_set_rotation_local(entt::entity entity, f32 value);
		static void transform_rotate_local(entt::entity entity, f32 value);
		



		static Vec2 sprite_get_texture_dimension(entt::entity entity);
		static u32 sprite_get_depth(entt::entity entity);


	private:
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
		static void _rigid_body_update_translation(entt::entity entity, Vec2 value);
		static void _rigid_body_update_rotation(entt::entity entity, f32 value);

		static void _transform_recompute_world_transform(entt::entity entity);
	};


}


using CUCA = mint::component::CUCA;


#endif