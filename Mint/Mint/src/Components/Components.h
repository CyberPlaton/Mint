#ifndef _MINT_COMPONENTS_H_
#define _MINT_COMPONENTS_H_


#include "Common/Common.h"
#include "Common/Rectangle.h"
#include "Graphics/Common/Color.h"
#include "Utility/Serialization/Serializer.h"

#include "Utility/Reflection/Reflection.h"


namespace mint::component
{

	struct SIdentifier INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SIdentifier);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);


		u64 m_uuid = 0;

		u64 m_enttId = 0;

		String m_debugName;
	};


	struct SSceneHierarchy INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SSceneHierarchy);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);


		entt::entity m_parent = entt::null;

		Vector< entt::entity > m_children;
	};


	struct SDynamicGameobject INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SDynamicGameobject);

		u8 m_placeholder = 0;
	};


	struct SRigidBody INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SRigidBody);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);


		b2Body* m_body = nullptr;

		b2PolygonShape m_bodyShape;

		b2FixtureDef m_fixtureDefinition;
	};


	struct STransform INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(STransform);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);


		Vec2 m_position = {0.0f, 0.0f};

		f32 m_rotation = 0.0f;

		Vec2 m_scale = { 0.0f, 0.0f };

		Mat4 m_worldTransform = Mat4(1.0f);
	};


	struct SSprite INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SSprite);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);


		bool m_visible = false;

		bool m_internalVisible = false;

		u64 m_depth = 0;

		CRect m_rect;

		bool m_flipX = false;

		bool m_flipY = false;

		mint::fx::CColor m_color;

		Vec2 m_origin;
	};


	struct SAnimatedSprite INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SAnimatedSprite);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);


		f32 m_frameCounter;

		u64 m_animationSpeed;
	};


	struct SScript INHERITS(mint::reflection::SBase)
	{
		REFLECTED_CLASS(SScript);

		static bool export_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);
		static bool import_component(entt::entity entity, entt::id_type hash, const entt::registry& registry, maml::SNode* node);


		ScriptHandle m_scriptHandle;
	};

}



#endif