#include "CUCA.h"


namespace mint::component
{
	MINT_CRITICAL_SECTION(CUCA::m_entityCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_scriptCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_animatedSpriteCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_spriteCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_rigidBodyCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_transformCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_hierarchyCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_identifierCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);
	


	mint::Vec2 CUCA::transform_get_position(entt::entity entity)
	{
		const auto& transform = CUCA::transform_get_world_transform_matrix(entity);

		const auto& position = Vec3(transform[3]);

		return { position.x, position.y };
	}


	mint::Vec2 CUCA::sprite_get_size(entt::entity entity)
	{
		const auto& materials = CUCA::sprite_get_all_materials(entity);

		// TODO: This does not take the sprite scale into account.

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto v = materials[0].get_texture_dimension();
		);

		return v;
	}


	mint::u64 CUCA::sprite_get_depth(entt::entity entity)
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

		_transform_recompute_world_transform(entity);
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

		_transform_recompute_world_transform(entity);

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

		_transform_recompute_world_transform(entity);
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

		_transform_recompute_world_transform(entity);

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

		_transform_recompute_world_transform(entity);
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

		_transform_recompute_world_transform(entity);

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

		_transform_recompute_world_transform(entity);
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

		_transform_recompute_world_transform(entity);

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


	f32 CUCA::transform_get_rotation(entt::entity entity)
	{
		const auto& transform = CUCA::transform_get_world_transform_matrix(entity);

		Vec3 rotation; glm::extractEulerAngleXYZ(transform, rotation.x, rotation.y, rotation.z);

		return mint::algorithm::radians_to_degree(rotation.z);
	}


	mint::Vec2 CUCA::transform_get_scale(entt::entity entity)
	{
		const auto& transform = CUCA::transform_get_world_transform_matrix(entity);	

		const auto& scale = Vec3(
			glm::length(Vec3(transform[0])),
			glm::length(Vec3(transform[1])),
			glm::length(Vec3(transform[2]))
		);

		return { scale.x, scale.y };
	}


	void CUCA::transform_scale(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to scale an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			transform.m_scale += value;

		);

		_transform_recompute_world_transform(entity);

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

		_transform_recompute_world_transform(entity);
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

		_transform_recompute_world_transform(entity);

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

		_transform_recompute_world_transform(entity);
	}


	mint::Mat4 CUCA::transform_get_world_transform_matrix(entt::entity entity)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true &&
					MINT_SCENE_REGISTRY().has_component< mint::component::SSceneHierarchy >(entity) == true,
			"Invalid operation. Trying to get the world Transform Matrix for an entity without a Transform or Scene Hierarchy component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		
		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			auto& value = transform.m_worldTransform;

		);

		return value;
	}


	mint::Mat4 CUCA::transform_get_local_transform_matrix(entt::entity entity)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to get the local Transform Matrix for an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);


		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			auto value = glm::translate(Mat4(1.0f), Vec3(transform.m_position, 0.0f)) *

								glm::rotate(Mat4(1.0f), mint::algorithm::degree_to_radians(transform.m_rotation), Vec3(0.0f, 0.0f, 1.0f)) *

								glm::scale(Mat4(1.0f), Vec3(transform.m_scale, 1.0f));
		);

		return value;
	}


	void CUCA::_transform_recompute_world_transform(entt::entity entity)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true &&
					MINT_SCENE_REGISTRY().has_component< mint::component::SSceneHierarchy >(entity) == true,
			"Invalid operation. Trying to recompute the world Transform Matrix for an entity without a Transform or Scene Hierarchy component!");


		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);
		const auto& hierarchy = MINT_SCENE_REGISTRY().get_component< mint::component::SSceneHierarchy >(entity);


		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			bool has_parent = hierarchy.m_parent != entt::null;
			entt::entity parent = hierarchy.m_parent;

		);


		if (has_parent)
		{
			transform.m_worldTransform = transform_get_world_transform_matrix(parent) * transform_get_local_transform_matrix(entity);
		}
		else
		{
			transform.m_worldTransform = transform_get_local_transform_matrix(entity);
		}
	}


	void CUCA::_rigid_body_update_scale(entt::entity entity, Vec2 value)
	{
		if (MINT_SCENE_REGISTRY().has_component< mint::component::SRigidBody >(entity))
		{
			auto& rb = MINT_SCENE_REGISTRY().get_component< mint::component::SRigidBody >(entity);

			rb.m_body->DestroyFixture(rb.m_body->GetFixtureList());

			const auto& dimensions = CUCA::sprite_get_size(entity);

			rb.m_bodyShape.SetAsBox(value.x * dimensions.x, value.y * dimensions.y);

			rb.m_fixtureDefinition.shape = &rb.m_bodyShape;

			rb.m_body->CreateFixture(&rb.m_fixtureDefinition);
		}
	}


	const mint::Vector< mint::fx::CMaterial >& CUCA::sprite_get_all_materials(entt::entity entity)
	{

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			const auto & v = fx::CMaterialManager::Get().get_materials_for_entity(entity);
			
		);

		return v;
	}


	void CUCA::sprite_add_material(entt::entity entity, const fx::CMaterial& material)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			fx::CMaterialManager::Get().add_material_for_entity(entity, material);

		);
	}

	mint::String CUCA::identifier_get_debug_name(entt::entity entity)
	{
		const auto& id = MINT_SCENE_REGISTRY().get_component< SIdentifier >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_identifierCriticalSection,

			auto name = id.m_debugName;

		);

		return name;
	}


	void CUCA::identifier_set_debug_name(entt::entity entity, const String& name)
	{
		auto& id = MINT_SCENE_REGISTRY().get_component< SIdentifier >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_identifierCriticalSection,

			id.m_debugName = name;

		);
	}

	u64 CUCA::identifier_get_identifier(entt::entity entity)
	{
		const auto& id = MINT_SCENE_REGISTRY().get_component< SIdentifier >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_identifierCriticalSection,

			auto entt_id = id.m_enttId;

		);

		return entt_id;
	}


	u64 CUCA::identifier_get_uuid(entt::entity entity)
	{
		const auto& id = MINT_SCENE_REGISTRY().get_component< SIdentifier >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_identifierCriticalSection,

			auto uuid = id.m_uuid;

		);

		return uuid;
	}


	bool CUCA::hierarchy_has_parent(entt::entity entity)
	{
		const auto& hierarchy = MINT_SCENE_REGISTRY().get_component< SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto result = hierarchy.m_parent != entt::null;

		);

		return result;
	}


	bool CUCA::hierarchy_has_children(entt::entity entity)
	{
		const auto& hierarchy = MINT_SCENE_REGISTRY().get_component< SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto result = !hierarchy.m_children.empty();

		);

		return result;
	}


	entt::entity CUCA::hierarchy_get_parent(entt::entity entity)
	{
		const auto& hierarchy = MINT_SCENE_REGISTRY().get_component< SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto parent = hierarchy.m_parent;

		);

		return parent;
	}


	mint::Vector< entt::entity > CUCA::hierarchy_get_children(entt::entity entity)
	{
		const auto& hierarchy = MINT_SCENE_REGISTRY().get_component< SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			auto kids = hierarchy.m_children;

		);

		return kids;
	}


	void CUCA::hierarchy_set_parent(entt::entity entity, entt::entity parent)
	{
		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			hierarchy.m_parent = parent;

		);
	}


	void CUCA::hierarchy_add_child(entt::entity entity, entt::entity child)
	{
		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			mint::algorithm::vector_push_back(hierarchy.m_children, child);

		);
	}


	void CUCA::hierarchy_remove_child(entt::entity entity, entt::entity child)
	{
		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< SSceneHierarchy >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_hierarchyCriticalSection,

			mint::algorithm::vector_erase(hierarchy.m_children, child);

		);
	}


	bool CUCA::sprite_is_visible(entt::entity entity)
	{
		const auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto value = sprite.m_visible;

		);

		return value;
	}


	void CUCA::sprite_set_is_visible(entt::entity entity, bool value)
	{
		auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			sprite.m_visible = value;

		);
	}


	bool CUCA::sprite_is_internal_visible(entt::entity entity)
	{
		const auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto value = sprite.m_internalVisible;

		);

		return value;
	}


	void CUCA::sprite_set_depth(entt::entity entity, u64 value)
	{
		auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			sprite.m_depth = value;

		);
	}


	mint::Vec2 CUCA::sprite_get_origin(entt::entity entity)
	{
		const auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto value = sprite.m_origin;

		);

		return value;
	}


	void CUCA::sprite_set_origin(entt::entity entity, const Vec2& value)
	{
		auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			sprite.m_origin = value;

		);
	}


	mint::CRect CUCA::sprite_get_source_rect(entt::entity entity)
	{
		const auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto value = sprite.m_rect;

		);

		return value;
	}


	void CUCA::sprite_set_source_rect(entt::entity entity, const CRect& value)
	{
		auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			sprite.m_rect = value;

		);
	}


	bool CUCA::sprite_is_flipped_x(entt::entity entity)
	{
		const auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto value = sprite.m_flipX;

		);

		return value;
	}


	void CUCA::sprite_set_is_flipped_x(entt::entity entity, bool value)
	{
		auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			sprite.m_flipX = value;

		);
	}


	bool CUCA::sprite_is_flipped_y(entt::entity entity)
	{
		const auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto value = sprite.m_flipY;

		);

		return value;
	}


	void CUCA::sprite_set_is_flipped_y(entt::entity entity, bool value)
	{
		auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			sprite.m_flipY = value;

		);
	}


	void CUCA::sprite_set_is_internal_visible(entt::entity entity, bool value)
	{
		auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			sprite.m_internalVisible = value;

		);
	}


	mint::fx::CColor CUCA::sprite_get_color(entt::entity entity)
	{
		const auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto value = sprite.m_color;

		);

		return value;
	}


	void CUCA::sprite_set_color(entt::entity entity, const fx::CColor& value)
	{
		auto& sprite = MINT_SCENE_REGISTRY().get_component< SSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			sprite.m_color = value;

		);
	}


	u64 CUCA::animated_sprite_get_animation_speed(entt::entity entity)
	{
		const auto& animsprite = MINT_SCENE_REGISTRY().get_component< SAnimatedSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_animatedSpriteCriticalSection,

			auto value = animsprite.m_animationSpeed;

		);

		return value;
	}


	void CUCA::animated_sprite_set_animation_speed(entt::entity entity, u64 value)
	{
		auto& animsprite = MINT_SCENE_REGISTRY().get_component< SAnimatedSprite >(entity);

		MINT_BEGIN_CRITICAL_SECTION(m_animatedSpriteCriticalSection,

			animsprite.m_animationSpeed = value;

		);
	}


	const mint::fx::CMaterial& CUCA::sprite_get_main_material(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			const auto & m = fx::CMaterialManager::Get().get_main_material_for_entity(entity);

		);

		return m;
	}


	mint::CRect CUCA::sprite_get_destination_rect(entt::entity entity)
	{
		auto position = CUCA::transform_get_position(entity);
		auto scale = CUCA::transform_get_scale(entity);
		auto size = CUCA::sprite_get_size(entity);

		return {
			position.x, position.y,
			position.x + size.x * scale.x, position.y + size.y * scale.y
		};
	}


}