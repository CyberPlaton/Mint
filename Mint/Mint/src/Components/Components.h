#ifndef _MINT_COMPONENTS_H_
#define _MINT_COMPONENTS_H_


#include "Common/Common.h"
#include "Common/Rectangle.h"
#include "Graphics/Common/Color.h"
#include "Utility/Serialization/Serializer.h"


namespace mint::component
{
	struct SSerializable
	{
		virtual void export_component(entt::entity entity, SSerializable* component, maml::SNode* node) {};

		virtual void import_component(entt::entity entity, SSerializable* component, maml::SNode* node) {};
	};

	struct SIdentifier : public SSerializable
	{
		virtual void export_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;
		virtual void import_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;


		u64 m_uuid = 0;

		u64 m_enttId = 0;

#ifndef MINT_DISTR
		String m_debugName;
#endif
	};


	struct SSceneHierarchy : public SSerializable
	{
		virtual void export_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;
		virtual void import_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;


		entt::entity m_parent = entt::null;

		Vector< entt::entity > m_children;
	};


	struct SDynamicGameobject : public SSerializable
	{
		u8 m_placeholder = 0;
	};


	struct SRigidBody : public SSerializable
	{
		virtual void export_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;
		virtual void import_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;


		b2Body* m_body = nullptr;

		b2PolygonShape m_bodyShape;

		b2FixtureDef m_fixtureDefinition;
	};


	struct STransform : public SSerializable
	{
		virtual void export_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;
		virtual void import_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;


		Vec2 m_position = {0.0f, 0.0f};

		f32 m_rotation = 0.0f;

		Vec2 m_scale = { 0.0f, 0.0f };

		Mat4 m_worldTransform = Mat4(1.0f);
	};


	struct SSprite : public SSerializable
	{
		virtual void export_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;
		virtual void import_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;


		bool m_visible = false;

		bool m_internalVisible = false;

		u64 m_depth = 0;

		CRect m_rect;

		bool m_flipX = false;

		bool m_flipY = false;

		mint::fx::CColor m_color;

		Vec2 m_origin;
	};


	struct SAnimatedSprite : public SSerializable
	{
		virtual void export_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;
		virtual void import_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;


		f32 m_frameCounter;

		u64 m_animationSpeed;
	};


	struct SScript : public SSerializable
	{
		virtual void export_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;
		virtual void import_component(entt::entity entity, SSerializable* component, maml::SNode* node) override;


		ScriptHandle m_scriptHandle;
	};

}



#endif