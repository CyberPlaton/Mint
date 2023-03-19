#ifndef _MINT_CUCA_H_
#define _MINT_CUCA_H_


#include "Components.h"

#include "SceneSystem/Common/IScene.h"


namespace mint::component
{
	class CUCA
	{
	public:

		static Vec2 transform_get_position(entt::entity entity);
		static f32 transform_get_rotation(entt::entity entity);
		static Vec2 transform_get_scale(entt::entity entity);
		static void transform_scale(entt::entity entity, Vec2 value);
		static void transform_scale_local(entt::entity entity, Vec2 value);
		static void transform_set_scale(entt::entity entity, Vec2 value);
		static void transform_set_scale_local(entt::entity entity, Vec2 value);
		static void transform_set_position_local(entt::entity entity, Vec2 value);
		static void transform_set_position(entt::entity entity, Vec2 value);
		static void transform_translate_local(entt::entity entity, Vec2 value);
		static void transform_translate(entt::entity entity, Vec2 value);
		static void transform_set_rotation_local(entt::entity entity, f32 value);
		static void transform_set_rotation(entt::entity entity, f32 value);
		static void transform_rotate_local(entt::entity entity, f32 value);
		static void transform_rotate(entt::entity entity, f32 value);



		static Vec2 sprite_get_texture_dimension(entt::entity entity);
		static u32 sprite_get_depth(entt::entity entity);


		static void dirtyflag_set_is_dirty(entt::entity entity, bool value);
		static bool dirtyflag_get_is_dirty(entt::entity entity);


	private:
		static MINT_CRITICAL_SECTION(m_dirtyFlagCriticalSection);
		static MINT_CRITICAL_SECTION(m_identifierCriticalSection);
		static MINT_CRITICAL_SECTION(m_hierarchyCriticalSection);
		static MINT_CRITICAL_SECTION(m_transformCriticalSection);
		static MINT_CRITICAL_SECTION(m_rigidBodyCriticalSection);
		static MINT_CRITICAL_SECTION(m_spriteCriticalSection);
		static MINT_CRITICAL_SECTION(m_animatedSpriteCriticalSection);
		static MINT_CRITICAL_SECTION(m_scriptCriticalSection);
		static MINT_CRITICAL_SECTION(m_dynamicGameobjectCriticalSection);

	private:
		static void _rigid_body_update_translation(entt::entity entity, Vec2 value);
		static void _rigid_body_update_rotation(entt::entity entity, f32 value);
	};


}


using CUCA = mint::component::CUCA;


#endif