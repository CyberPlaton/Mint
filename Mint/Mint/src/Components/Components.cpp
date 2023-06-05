#include "Components.h"


namespace mint::component
{


	bool SIdentifier::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SIdentifier >().hash() != hash) return true;

		const auto& identifier = registry.get< SIdentifier >(entity);

		CSerializer::export_uint(identifier.m_enttId, "id", node);
		CSerializer::export_uint(identifier.m_uuid, "uuid", node);

#ifndef MINT_DISTR
		CSerializer::export_string(identifier.m_debugName, "name", node);
#endif

		return true;
	}


	bool SIdentifier::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SIdentifier >().hash() != hash) return true;

		auto& identifier = registry.get< SIdentifier >(entity);

		CSerializer::import_uint(&identifier.m_enttId, "id", node, -1);
		CSerializer::import_uint(&identifier.m_uuid, "uuid", node, -1);
		CSerializer::import_string(identifier.m_debugName, "name", node);

		return true;
	}


	bool SSceneHierarchy::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SSceneHierarchy >().hash() != hash) return true;

		const auto& hierarchy = registry.get< SSceneHierarchy >(entity);

		entt::entity parent = hierarchy.m_parent;
		if(parent == entt::null)
		{
			CSerializer::export_sint(-1, "parent", node);
		}
		else
		{
			CSerializer::export_uint(SCAST(u64, parent), "parent", node);
		}

		Vector< CAny > kids;
		if(!hierarchy.m_children.empty())
		{
			for (const auto& entity : hierarchy.m_children) kids.emplace_back(SCAST(u64, entity));
		}

		CSerializer::export_array(kids, "children", node);

		return true;
	}


	bool SSceneHierarchy::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SSceneHierarchy >().hash() != hash) return true;

		auto& hierarchy = registry.get< SSceneHierarchy >(entity);

		Vector< CAny > children;
		u64 parent;

		CSerializer::import_array(children, "children", node);
		CSerializer::import_uint(&parent, "parent", node, -1);

		hierarchy.m_parent = (entt::entity)parent;
		for (auto& kid : children)
		{
			mint::algorithm::vector_push_back(hierarchy.m_children, kid.cast< entt::entity >());
		}

		return true;
	}


	bool SRigidBody::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SRigidBody >().hash() != hash) return true;

		const auto& rigid_body = registry.get< SRigidBody >(entity);

		MINT_LOG_CRITICAL("[{:.4f}][SRigidBody::export_component] Exporting SRigidBody is not supported yet!", MINT_APP_TIME);

		return false;
	}


	bool SRigidBody::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SRigidBody >().hash() != hash) return true;

		MINT_LOG_CRITICAL("[{:.4f}][SRigidBody::import_component] Importing SRigidBody is not supported yet!", MINT_APP_TIME);

		return false;
	}


	bool STransform::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::STransform >().hash() != hash) return true;

		const auto& transform = registry.get< STransform >(entity);

		Vec2 position = transform.m_position;
		Vec2 scale = transform.m_scale;

		CSerializer::export_vec2(position, "position", node);
		CSerializer::export_vec2(scale, "scale", node);
		CSerializer::export_float(transform.m_rotation, "rotation", node);

		return true;
	}


	bool STransform::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::STransform >().hash() != hash) return true;

		auto& transform = registry.get< STransform >(entity);

		CSerializer::import_vec2(transform.m_position, "position", node);
		CSerializer::import_vec2(transform.m_scale, "scale", node);
		CSerializer::import_float(&transform.m_rotation, "rotation", node);

		return true;
	}


	bool SSprite::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SSprite >().hash() != hash) return true;

		const auto& sprite = registry.get< SSprite >(entity);

		Vec2 origin = sprite.m_origin;

		CSerializer::export_bool(sprite.m_visible, "visible", node);
		CSerializer::export_bool(sprite.m_internalVisible, "internal_visible", node);
		CSerializer::export_uint(sprite.m_depth, "depth", node);
		CSerializer::export_bool(sprite.m_flipX, "flip_x", node);
		CSerializer::export_bool(sprite.m_flipY, "flip_y", node);
		CSerializer::export_vec4(sprite.m_color.as_vec4(), "color", node);
		CSerializer::export_vec2(origin, "origin", node);

		return true;
	}


	bool SSprite::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SSprite >().hash() != hash) return true;

		auto& sprite = registry.get< SSprite >(entity);

		Vec4 color;

		CSerializer::import_bool(&sprite.m_visible, "visible", node);
		CSerializer::import_bool(&sprite.m_internalVisible, "internal_visible", node);
		CSerializer::import_uint(&sprite.m_depth, "depth", node, -1);
		CSerializer::import_bool(&sprite.m_flipX, "flip_x", node);
		CSerializer::import_bool(&sprite.m_flipY, "flip_y", node);
		CSerializer::import_vec4(color, "color", node);
		CSerializer::import_vec2(sprite.m_origin, "origin", node);

		sprite.m_color = color;

		return true;
	}


	bool SAnimatedSprite::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SAnimatedSprite >().hash() != hash) return true;

		const auto& animated_sprite = registry.get< SAnimatedSprite >(entity);

		CSerializer::export_float(animated_sprite.m_frameCounter, "frame_counter", node);
		CSerializer::export_uint(animated_sprite.m_animationSpeed, "animation_speed", node);

		return true;
	}


	bool SAnimatedSprite::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SAnimatedSprite >().hash() != hash) return true;

		auto& animsprite = registry.get< SAnimatedSprite >(entity);

		return true;
	}


	bool SScript::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SScript >().hash() != hash) return true;

		const auto& script = registry.get< SScript >(entity);

		CSerializer::export_uint(script.m_scriptHandle, "script_handle", node);

		return true;
	}


	bool SScript::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SScript >().hash() != hash) return true;

		auto& script = registry.get< SScript >(entity);

		CSerializer::import_uint(&script.m_scriptHandle, "script_handle", node, -1);

		return true;
	}


}