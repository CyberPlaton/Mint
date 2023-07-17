#include "Components.h"


namespace mint::component
{

	bool SSoundSource::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SSoundSource >().hash() != hash) return true;

		const auto& ss = registry.get< SSoundSource >(entity);

		CSerializer::export_uint(ss.m_soundHandle, "soundhandle", node);
		CSerializer::export_bool(ss.m_3dTo2dMorphing, "morphing", node);
		CSerializer::export_bool(ss.m_loopmode, "loopmode", node);
		CSerializer::export_float(ss.m_volume, "volume", node);
		CSerializer::export_float(ss.m_minDistance, "mindistance", node);
		CSerializer::export_float(ss.m_maxDistance, "maxdistance", node);
		CSerializer::export_float(ss.m_pitch, "pitch", node);
		CSerializer::export_float(ss.m_pan, "pan", node);
		CSerializer::export_float(ss.m_height, "height", node);
		CSerializer::export_uint(SCAST(u64, ss.m_mode), "mode", node);
		CSerializer::export_vec3(ss.m_velocity, "velocity", node);

		return true;
	}


	bool SSoundSource::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SSoundSource >().hash() != hash ||
			registry.all_of< mint::component::SSoundSource >(entity)) return true;

		auto& ss = registry.emplace< SSoundSource >(entity);

		CSerializer::import_uint(&ss.m_soundHandle, "soundhandle", node);
		CSerializer::import_bool(&ss.m_3dTo2dMorphing, "morphing", node, true);
		CSerializer::import_bool(&ss.m_loopmode, "loopmode", node, false);
		CSerializer::import_float(&ss.m_volume, "volume", node, 1.0f);
		CSerializer::import_float(&ss.m_minDistance, "mindistance", node, 1.0f);
		CSerializer::import_float(&ss.m_maxDistance, "maxdistance", node, 1.0f);
		CSerializer::import_float(&ss.m_pitch, "pitch", node, 1.0f);
		CSerializer::import_float(&ss.m_pan, "pan", node, 1.0f);
		CSerializer::import_float(&ss.m_height, "height", node, 1.0f);
		CSerializer::import_vec3(ss.m_velocity, "velocity", node);

		u64 mode = 0;
		CSerializer::import_uint(&mode, "mode", node);

		ss.m_mode = SCAST(FMOD_MODE, mode);


#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&ss);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif

		return true;
	}


	bool SParticleEmitter::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SParticleEmitter >().hash() != hash) return true;

		const auto& pe = registry.get< SParticleEmitter >(entity);

		return true;
	}


	bool SParticleEmitter::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SParticleEmitter >().hash() != hash ||
			registry.all_of< mint::component::SParticleEmitter >(entity)) return true;

		auto& pe = registry.emplace< SParticleEmitter >(entity);

#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&pe);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif

		return true;
	}


	bool SWorldSettings::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SWorldSettings >().hash() != hash) return true;

		const auto& ws = registry.get< SWorldSettings >(entity);

		CSerializer::export_uint(ws.m_groupId, "group", node);
		CSerializer::export_bool(ws.m_enabled, "enabled", node);
		CSerializer::export_bool(ws.m_filterEnabled, "filter", node);
		CSerializer::export_bool(ws.m_queryable, "query", node);

		return true;
	}


	bool SWorldSettings::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SWorldSettings >().hash() != hash ||
			registry.all_of< mint::component::SWorldSettings >(entity)) return true;

		auto& ws = registry.emplace< SWorldSettings >(entity);

#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&ws);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif


		CSerializer::import_uint(&ws.m_groupId, "group", node, 0);
		CSerializer::import_bool(&ws.m_enabled, "enabled", node, true);
		CSerializer::import_bool(&ws.m_filterEnabled, "filter", node, true);
		CSerializer::import_bool(&ws.m_queryable, "query", node, true);

		return true;
	}


	bool SIdentifier::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SIdentifier >().hash() != hash) return true;

		const auto& identifier = registry.get< SIdentifier >(entity);

		CSerializer::export_uint(identifier.m_enttId, "id", node);
		CSerializer::export_uint(identifier.m_uuid, "uuid", node);
		CSerializer::export_string(identifier.m_debugName, "name", node);

		return true;
	}


	bool SIdentifier::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SIdentifier >().hash() != hash ||
			registry.all_of< mint::component::SIdentifier >(entity)) return true;

		auto& identifier = registry.emplace< SIdentifier >(entity);


#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&identifier);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif


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
		// Skip importing duplicate or invalid components.
		if (entt::type_id< mint::component::SSceneHierarchy >().hash() != hash ||
		    registry.all_of< mint::component::SSceneHierarchy >(entity)) return true;

		auto& hierarchy = registry.emplace< SSceneHierarchy >(entity);


#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&hierarchy);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif



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
		// Skip importing duplicate or invalid components.
		if (entt::type_id< mint::component::SRigidBody >().hash() != hash ||
			registry.all_of< mint::component::SRigidBody >(entity)) return true;

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
		if (entt::type_id< mint::component::STransform >().hash() != hash ||
			registry.all_of< mint::component::STransform >(entity)) return true;

		auto& transform = registry.emplace< STransform >(entity);


#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&transform);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif


		CSerializer::import_vec2(transform.m_position, "position", node);
		CSerializer::import_vec2(transform.m_scale, "scale", node);
		CSerializer::import_float(&transform.m_rotation, "rotation", node);


		transform.m_worldTransform = glm::translate(Mat4(1.0f), Vec3(transform.m_position, 0.0f)) *

									 glm::rotate(Mat4(1.0f), transform.m_rotation, Vec3(0.0f, 0.0f, 1.0f)) *

									 glm::scale(Mat4(1.0f), Vec3(transform.m_scale, 0.0f));


		return true;
	}


	bool SSprite::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SSprite >().hash() != hash) return true;

		const auto& sprite = registry.get< SSprite >(entity);

		Vec2 origin = sprite.m_origin;

		CSerializer::export_uint(sprite.m_visible == true ? 1 : 0, "visible", node);
		CSerializer::export_uint(sprite.m_internalVisible == true ? 1 : 0, "internal_visible", node);
		CSerializer::export_uint(sprite.m_depth, "depth", node);
		CSerializer::export_bool(sprite.m_flipX, "flip_x", node);
		CSerializer::export_bool(sprite.m_flipY, "flip_y", node);
		CSerializer::export_vec4(sprite.m_color.as_vec4(), "color", node);
		CSerializer::export_vec2(origin, "origin", node);

		return true;
	}


	bool SSprite::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SSprite >().hash() != hash ||
			registry.all_of< mint::component::SSprite >(entity)) return true;

		auto& sprite = registry.emplace< SSprite >(entity);


#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&sprite);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif



		Vec4 color;
		u64 visible, internal_visible;

		CSerializer::import_uint(&visible, "visible", node);
		CSerializer::import_uint(&internal_visible, "internal_visible", node);
		CSerializer::import_uint(&sprite.m_depth, "depth", node, -1);
		CSerializer::import_bool(&sprite.m_flipX, "flip_x", node);
		CSerializer::import_bool(&sprite.m_flipY, "flip_y", node);
		CSerializer::import_vec4(color, "color", node);
		CSerializer::import_vec2(sprite.m_origin, "origin", node);

		sprite.m_color = color;
		sprite.m_visible = visible == 1 ? true : false;
		sprite.m_internalVisible = internal_visible == 1 ? true : false;

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
		if (entt::type_id< mint::component::SAnimatedSprite >().hash() != hash ||
			registry.all_of< mint::component::SAnimatedSprite >(entity)) return true;

		auto& animsprite = registry.emplace< SAnimatedSprite >(entity);


#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&animsprite);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif



		return true;
	}


	bool SScript::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SScript >().hash() != hash) return true;

		return true;
	}


	bool SScript::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SScript >().hash() != hash ||
			registry.all_of< mint::component::SScript >(entity)) return true;

		auto& script = registry.emplace< SScript >(entity);


#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&script);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif

		return true;
	}


	bool SDynamicGameobject::export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node)
	{
		// Do nothing. That the entity has this component will be added to an Array.
		return true;
	}

	bool SDynamicGameobject::import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node)
	{
		if (entt::type_id< mint::component::SDynamicGameobject >().hash() != hash ||
			registry.all_of< mint::component::SDynamicGameobject >(entity)) return true;

		// Merely add the component.
		auto& dynamic = registry.emplace< SDynamicGameobject >(entity);


#if MINT_DISTR
#else
		auto metaclass = mint::reflection::SBase::get_metaclass(&dynamic);
		metaclass->set_metaclass_entity(entity);

		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(u64, entity), metaclass);
#endif


		return true;
	}

}