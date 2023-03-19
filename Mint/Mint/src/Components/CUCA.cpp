#include "CUCA.h"


namespace mint::component
{

	MINT_CRITICAL_SECTION(CUCA::m_scriptCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_animatedSpriteCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_spriteCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_rigidBodyCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_transformCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_hierarchyCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_identifierCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_dirtyFlagCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);
	


	mint::Vec2 CUCA::transform_get_position(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			Vec2 v = MINT_SCENE_REGISTRY().get_component< STransform >(entity).m_position;

		);

		return v;
	}


	mint::Vec2 CUCA::sprite_get_texture_dimension(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto w = MINT_SCENE_REGISTRY().get_component< SSprite >(entity).m_textureWidth;
			auto h = MINT_SCENE_REGISTRY().get_component< SSprite >(entity).m_textureHeight;

		);

		return { w, h };
	}


	mint::u32 CUCA::sprite_get_depth(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto value = MINT_SCENE_REGISTRY().get_component< SSprite >(entity).m_depth;
		
		);

		return value;
	}


	void CUCA::transform_set_position_local(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to translate an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		Vec2 position = value;

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_position = value;
		);

		_rigid_body_update_translation(entity, position);

		dirtyflag_set_is_dirty(entity, true);
	}


	void CUCA::transform_set_position(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to translate an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		Vec2 position = value;
		Vec2 prev_position;

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			prev_position = transform.m_position;

			transform.m_position = value;
		);

		_rigid_body_update_translation(entity, position);

		dirtyflag_set_is_dirty(entity, true);

		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< mint::component::SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto & children = hierarchy.m_children;

		);

		for (auto& kid : children)
		{
			Vec2 relative = prev_position - transform_get_position(kid);
				
			transform_translate(kid, { position.x + relative.x,  position.y + relative.y });
		}
	}


	void CUCA::transform_translate_local(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to translate an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		Vec2 position;

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_position += value;

			position = transform.m_position;
		);

		_rigid_body_update_translation(entity, position);

		dirtyflag_set_is_dirty(entity, true);
	}


	void CUCA::transform_translate(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
					"Invalid operation. Trying to translate an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		Vec2 position;

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_position += value;
			
			position = transform.m_position;
		);

		_rigid_body_update_translation(entity, position);

		dirtyflag_set_is_dirty(entity, true);

		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< mint::component::SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto & children = hierarchy.m_children;
			
		);

		for(auto& kid: children)
		{
			transform_translate(kid, value);
		}
	}


	void CUCA::transform_set_rotation_local(entt::entity entity, f32 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to rotate an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		f32 rotation = value;

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_rotation = value;

		);

		_rigid_body_update_rotation(entity, rotation);

		dirtyflag_set_is_dirty(entity, true);
	}


	void CUCA::transform_set_rotation(entt::entity entity, f32 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to rotate an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		f32 rotation = value;
		f32 prev_rotation;

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			prev_rotation = transform.m_rotation;

			transform.m_rotation = value;

		);

		_rigid_body_update_rotation(entity, rotation);

		dirtyflag_set_is_dirty(entity, true);

		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< mint::component::SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto & children = hierarchy.m_children;

		);

		for (auto& kid : children)
		{
			f32 relative = prev_rotation - transform_get_rotation(entity);
			transform_rotate(kid, value+ relative);
		}
	}


	void CUCA::transform_rotate_local(entt::entity entity, f32 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to rotate an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		f32 rotation;

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_rotation += value;

		rotation = transform.m_rotation;
		);

		_rigid_body_update_rotation(entity, rotation);

		dirtyflag_set_is_dirty(entity, true);
	}


	void CUCA::transform_rotate(entt::entity entity, f32 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to rotate an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		f32 rotation;

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_rotation += value;

			rotation = transform.m_rotation;
		);

		_rigid_body_update_rotation(entity, rotation);

		dirtyflag_set_is_dirty(entity, true);

		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< mint::component::SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto & children = hierarchy.m_children;

		);

		for (auto& kid : children)
		{
			transform_rotate(kid, value);
		}
	}


	void CUCA::_rigid_body_update_translation(entt::entity entity, Vec2 value)
	{
		if(MINT_SCENE_REGISTRY().has_component< mint::component::SRigidBody >(entity))
		{
			auto& rb = MINT_SCENE_REGISTRY().get_component< mint::component::SRigidBody >(entity);

			MINT_BEGIN_CRITICAL_SECTION(m_rigidBodyCriticalSection,

				rb.m_body->SetTransform({ value.x, value.y }, rb.m_body->GetTransform().q.GetAngle());

			);
		}
	}


	void CUCA::_rigid_body_update_rotation(entt::entity entity, f32 value)
	{
		if (MINT_SCENE_REGISTRY().has_component< mint::component::SRigidBody >(entity))
		{
			auto& rb = MINT_SCENE_REGISTRY().get_component< mint::component::SRigidBody >(entity);

			MINT_BEGIN_CRITICAL_SECTION(m_rigidBodyCriticalSection,

				rb.m_body->SetTransform(rb.m_body->GetTransform().p, glm::radians(value));

			);
		}
	}


	void CUCA::dirtyflag_set_is_dirty(entt::entity entity, bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_dirtyFlagCriticalSection,

			MINT_SCENE_REGISTRY().get_component< mint::component::SDirty >(entity).m_isDirty = value;
		);
	}


	bool CUCA::dirtyflag_get_is_dirty(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_dirtyFlagCriticalSection,

			auto v = MINT_SCENE_REGISTRY().get_component< mint::component::SDirty >(entity).m_isDirty;

		);

		return v;
	}


	f32 CUCA::transform_get_rotation(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			auto v = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity).m_rotation;

		);

		return v;
	}


	mint::Vec2 CUCA::transform_get_scale(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			auto v = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity).m_scale;

		);

		return v;
	}


	void CUCA::transform_scale(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to scale an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_scale += value;

		);

		dirtyflag_set_is_dirty(entity, true);

		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< mint::component::SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto & children = hierarchy.m_children;

		);

		for (auto& kid : children)
		{
			transform_scale(kid, value);
		}
	}


	void CUCA::transform_scale_local(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to scale an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_scale += value;

		);

		dirtyflag_set_is_dirty(entity, true);
	}


	void CUCA::transform_set_scale(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to scale an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		Vec2 prev_scale;


		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			prev_scale = transform.m_scale;

			transform.m_scale = value;

		);

		dirtyflag_set_is_dirty(entity, true);

		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< mint::component::SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto & children = hierarchy.m_children;

		);

		for (auto& kid : children)
		{
			Vec2 relative = prev_scale - transform_get_scale(kid);

			transform_scale(kid, {value.x + relative.x, value.y + relative.y });
		}
	}


	void CUCA::transform_set_scale_local(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to scale an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_scale = value;

		);

		dirtyflag_set_is_dirty(entity, true);
	}


}