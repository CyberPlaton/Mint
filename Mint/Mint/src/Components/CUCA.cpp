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
		const auto& transform = CUCA::_get_world_transform(entity);

		return CUCA::extract_position_from_world_transform(transform);
	}


	mint::Vec2 CUCA::sprite_get_size(entt::entity entity)
	{
		auto mat = fx::CMaterialManager::Get().get_main_material_for_entity(entity);
		
		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto v = mat->get_texture_dimension();
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


	void CUCA::transform_set_position(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to translate an entity without a Transform component!");

		// Update data.
		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);
		
		transform.m_position = value;

		// Update transform matrix and children if any.
		_update_world_transform_recursive(entity);

		// Update physics data.
		_rigid_body_update_translation(entity, transform.m_position);
	}


	void CUCA::transform_translate(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
					"Invalid operation. Trying to translate an entity without a Transform component!");

		// Set new data value.
		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);
		
		transform.m_position += value;

		// Update transform matrix and children if any.
		_update_world_transform_recursive(entity);

		// Update physics data.
		_rigid_body_update_translation(entity, transform.m_position);
	}


	void CUCA::transform_set_rotation(entt::entity entity, f32 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to rotate an entity without a Transform component!");

		// Update the data.
		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);
		
		transform.m_rotation = value;

		// Update transform matrix and children if any.
		_update_world_transform_recursive(entity);

		// Update physics data.
		_rigid_body_update_rotation(entity, transform.m_rotation);
	}


	void CUCA::transform_rotate(entt::entity entity, f32 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to rotate an entity without a Transform component!");

		// Update data.
		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);
		
		transform.m_rotation += value;

		// Update transform matrix and children if any.
		_update_world_transform_recursive(entity);

		// Update physics if required.
		_rigid_body_update_rotation(entity, transform.m_rotation);
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

				rb.m_body->SetTransform(rb.m_body->GetTransform().p, value);

			);
		}
	}


	f32 CUCA::transform_get_rotation(entt::entity entity)
	{
		const auto& transform = CUCA::_get_world_transform(entity);

		return CUCA::extract_rotation_from_world_transform(transform);
	}


	mint::Vec2 CUCA::transform_get_scale(entt::entity entity)
	{
		const auto& transform = CUCA::_get_world_transform(entity);

		return CUCA::extract_scale_from_world_transform(transform);
	}


	void CUCA::transform_scale(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to scale an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		// Set new value.
		transform.m_scale += value;

		// Update transform matrix and children if any.
		_update_world_transform_recursive(entity);

		// Update physics if required.
		_rigid_body_update_scale(entity, transform.m_scale);
	}


	void CUCA::transform_set_scale(entt::entity entity, Vec2 value)
	{
		MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::STransform >(entity) == true,
			"Invalid operation. Trying to scale an entity without a Transform component!");

		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		// Set new value.
		transform.m_scale = value;

		// Update transform matrix and children if any.
		_update_world_transform_recursive(entity);

		// Update physics if required.
		_rigid_body_update_scale(entity, transform.m_scale);
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


	mint::CMap2< mint::fx::CMaterial >& CUCA::sprite_get_all_materials(entt::entity entity)
	{

		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto & v = fx::CMaterialManager::Get().get_materials_for_entity(entity);
			
		);

		return v;
	}


	void CUCA::sprite_add_material(entt::entity entity, const fx::SMaterialDefinition& material_definition)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			fx::CMaterialManager::Get().add_material_for_entity(entity, material_definition);

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

			CUCA::hierarchy_set_parent(child, entt::null);

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


	mint::fx::CMaterial* CUCA::sprite_get_main_material(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto m = fx::CMaterialManager::Get().get_main_material_for_entity(entity);

		);

		return m;
	}


	mint::CRect CUCA::sprite_get_destination_rect(entt::entity entity)
	{
		auto& mat = CUCA::_get_world_transform(entity);

		auto position = CUCA::extract_position_from_world_transform(mat);
		auto scale = CUCA::extract_scale_from_world_transform(mat);
		auto size = CUCA::sprite_get_size(entity);
		auto origin = CUCA::sprite_get_origin(entity);


		MINT_ASSERT((std::isnan(position.x) || std::isnan(position.y)) == false,
					"Invalid operation. Position of an Entity became invalid or was not set correctly!");

		MINT_ASSERT((std::isnan(scale.x) || std::isnan(scale.y)) == false,
			"Invalid operation. Scale of an Entity became invalid or was not set correctly!");

		MINT_ASSERT((std::isnan(size.x) || std::isnan(size.y)) == false,
			"Invalid operation. Texture size of an Entity became invalid or was not set correctly!");

		MINT_ASSERT((std::isnan(origin.x) || std::isnan(origin.y)) == false,
			"Invalid operation. Texture origin of an Entity became invalid or was not set correctly!");

		f32 x = position.x - origin.x * scale.x;
		f32 y = position.y - origin.y * scale.y;

		return {
			x, y,
			size.x * scale.x, size.y * scale.y
 		};

	}


	mint::Vec2 CUCA::extract_position_from_world_transform(const Mat4& transform)
	{
		const auto& position = Vec3(transform[3]);

		return { position.x, position.y };
	}


	f32 CUCA::extract_rotation_from_world_transform(const Mat4& transform)
	{
		Vec3 rotation; glm::extractEulerAngleXYZ(transform, rotation.x, rotation.y, rotation.z);

		return rotation.z;
	}


	mint::Vec2 CUCA::extract_scale_from_world_transform(const Mat4& transform)
	{	
		const auto& scale = Vec3(
			glm::length(Vec3(transform[0])),
			glm::length(Vec3(transform[1])),
			glm::length(Vec3(transform[2]))
		);

		return { scale.x, scale.y };
	}


	mint::Mat4 CUCA::_get_local_transform(entt::entity entity)
	{
		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		auto value = glm::translate(Mat4(1.0f), Vec3(transform.m_position, 0.0f)) *

					 glm::rotate(Mat4(1.0f), transform.m_rotation, Vec3(0.0f, 0.0f, 1.0f)) *

					 glm::scale(Mat4(1.0f), Vec3(transform.m_scale, 0.0f));

		return value;
	}


	mint::Mat4 CUCA::_get_world_transform(entt::entity entity)
	{
		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		if( CUCA::hierarchy_has_parent(entity))
		{
			auto& parent_transform = CUCA::_get_world_transform(CUCA::hierarchy_get_parent(entity));

			return parent_transform * transform.m_worldTransform;
		}

		return transform.m_worldTransform;
	}


	void CUCA::_update_world_transform_recursive(entt::entity entity)
	{
		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);
		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< mint::component::SSceneHierarchy >(entity);

		// Update own transform matrix.
		transform.m_worldTransform = _get_local_transform(entity);


		// Extract data from newly computed transform.
		transform.m_position = CUCA::extract_position_from_world_transform(transform.m_worldTransform);
		transform.m_scale = CUCA::extract_scale_from_world_transform(transform.m_worldTransform);
 		transform.m_rotation = CUCA::extract_rotation_from_world_transform(transform.m_worldTransform);


		if( CUCA::hierarchy_has_children(entity))
		{
			auto& children = hierarchy.m_children;

			for(auto& kid : children)
			{
				_update_world_transform_recursive(kid);
			}
		}
	}


	void CUCA::hierarchy_remove_parent(entt::entity entity)
	{
		auto& hierarchy = MINT_SCENE_REGISTRY().get_component< SSceneHierarchy >(entity);

		CUCA::hierarchy_remove_child(hierarchy.m_parent, entity);
	}


	void CUCA::transform_set_transform_matrix(entt::entity entity, const Mat4& matrix)
	{
		auto& transform = MINT_SCENE_REGISTRY().get_component< mint::component::STransform >(entity);

		transform.m_worldTransform = matrix;
	}


}