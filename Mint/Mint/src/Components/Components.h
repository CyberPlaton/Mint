#ifndef _MINT_COMPONENTS_H_
#define _MINT_COMPONENTS_H_


#include "Common/Common.h"
#include "Common/Rectangle.h"
#include "Graphics/Common/Color.h"
#include "Utility/Serialization/Serializer.h"
#include "Utility/Logging/Logging.h"
#include "Utility/Reflection/Reflection.h"

#if MINT_DISTR
#else
#include "Utility/Reflection/EntityMetaclassDatabase.h"
#endif


namespace mint::component
{

	struct SIdentifier INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SIdentifier);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node);


		u64 m_uuid = 0;

		u64 m_enttId = 0;

		u64 m_groupId = 0;

		String m_debugName;
	};


	struct SSceneHierarchy INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SSceneHierarchy);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node);


		entt::entity m_parent = entt::null;

		Vector< entt::entity > m_children;
	};


	struct SDynamicGameobject INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SDynamicGameobject);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node);


		u8 m_placeholder = 0;
	};


	struct SRigidBody INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SRigidBody);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node);


		b2Body* m_body = nullptr;

		b2PolygonShape m_bodyShape;

		b2FixtureDef m_fixtureDefinition;
	};


	struct STransform INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(STransform);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node);


		Vec2 m_position = {0.0f, 0.0f};

		/// @brief Rotation in Radians. 
		f32 m_rotation = 0.0f;

		Vec2 m_scale = { 0.0f, 0.0f };

		bool m_dirty = false;

		Mat4 m_worldTransform = Mat4(1.0f);
	};


	struct SSprite INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SSprite);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node);


		bool m_visible = true;

		bool m_internalVisible = true;

		u64 m_depth = 0;

		CRect m_rect = {0.0f, 0.0f, 0.0f, 0.0f };

		bool m_flipX = false;

		bool m_flipY = false;

		mint::fx::CColor m_color = {255, 255, 255, 255};

		Vec2 m_origin = { 0.0f, 0.0f };
	};


	struct SAnimatedSprite INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SAnimatedSprite);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node);


		f32 m_frameCounter = 0;

		u64 m_animationSpeed = 0;
	};


	struct SScript INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SScript);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, entt::registry& registry, maml::SNode* node);


		ScriptHandle m_scriptHandle = MINT_INVALID_HANDLE;
	};

}



#endif