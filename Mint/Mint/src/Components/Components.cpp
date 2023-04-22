#include "Components.h"


namespace mint::component
{


	void SIdentifier::export_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{
		auto identifier = reinterpret_cast<SIdentifier*>(component);

		CSerializer::export_uint(identifier->m_enttId, "id", node);
		CSerializer::export_uint(identifier->m_uuid, "uuid", node);

#ifndef MINT_DISTR
		CSerializer::export_string(identifier->m_debugName, "name", node);
#endif
	}


	void SIdentifier::import_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{

	}


	void SSceneHierarchy::export_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{
		auto hierarchy = reinterpret_cast<SSceneHierarchy*>(component);

		entt::entity parent = hierarchy->m_parent;
		if(parent == entt::null)
		{
			CSerializer::export_sint(-1, "parent", node);
		}
		else
		{
			CSerializer::export_uint(SCAST(u64, parent), "parent", node);
		}

		Vector< CAny > kids;
		if(!hierarchy->m_children.empty())
		{
			for (const auto& entity : hierarchy->m_children) kids.emplace_back(SCAST(u64, entity));
		}

		CSerializer::export_array(kids, "children", node);
	}


	void SSceneHierarchy::import_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{

	}


	void SRigidBody::export_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{
		auto rigid_body = reinterpret_cast<SRigidBody*>(component);
	}


	void SRigidBody::import_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{

	}


	void STransform::export_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{
		auto transform = reinterpret_cast<STransform*>(component);

		CSerializer::export_vec2(transform->m_position, "position", node);
		CSerializer::export_vec2(transform->m_scale, "scale", node);
		CSerializer::export_float(transform->m_rotation, "rotation", node);
	}


	void STransform::import_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{

	}


	void SSprite::export_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{
		auto sprite = reinterpret_cast<SSprite*>(component);

		CSerializer::export_bool(sprite->m_visible, "visible", node);
		CSerializer::export_bool(sprite->m_internalVisible, "internal_visible", node);
		CSerializer::export_uint(sprite->m_depth, "depth", node);
		CSerializer::export_bool(sprite->m_flipX, "flip_x", node);
		CSerializer::export_bool(sprite->m_flipY, "flip_y", node);
		CSerializer::export_vec4(sprite->m_color.as_vec4(), "color", node);
		CSerializer::export_vec2(sprite->m_origin, "origin", node);
	}


	void SSprite::import_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{

	}


	void SAnimatedSprite::export_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{
		auto animated_sprite = reinterpret_cast<SAnimatedSprite*>(component);

		CSerializer::export_float(animated_sprite->m_frameCounter, "frame_counter", node);
		CSerializer::export_uint(animated_sprite->m_animationSpeed, "animation_speed", node);
	}


	void SAnimatedSprite::import_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{

	}


	void SScript::export_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{
		auto script = reinterpret_cast<SScript*>(component);

		CSerializer::export_uint(script->m_scriptHandle, "script_handle", node);
	}


	void SScript::import_component(entt::entity entity, SSerializable* component, maml::SNode* node)
	{

	}


}