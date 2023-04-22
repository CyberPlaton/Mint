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
	MINT_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);
	


	mint::Vec2 CUCA::transform_get_position(entt::entity entity)
	{
		const auto& transform = CUCA::transform_get_world_transform_matrix(entity);

		const auto& position = Vec3(transform[3]);

		return { position.x, position.y };
	}


	mint::Vec2 CUCA::sprite_get_texture_dimension(entt::entity entity)
	{
		const auto& materials = CUCA::sprite_get_all_materials(entity);


		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto v = materials[0].get_texture_dimension();
		);

		return v;
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

		return rotation.z;
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

			const auto& dimensions = CUCA::sprite_get_texture_dimension(entity);

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


}