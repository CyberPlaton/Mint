#ifndef _MINT_COMPONENTS_H_
#define _MINT_COMPONENTS_H_


#include "Common/Common.h"
#include "Common/Rectangle.h"
#include "Graphics/Common/Color.h"

namespace mint::component
{

	struct SIdentifier
	{
		u64 m_uuid = 0;

		u64 m_enttId = 0;

#ifndef MINT_DISTR
		String m_debugName;
#endif
	};


	struct SSceneHierarchy
	{
		entt::entity m_parent = entt::null;

		Vector< entt::entity > m_children;
	};


	struct SDynamicGameobject
	{
		u8 m_placeholder = 0;
	};


	struct SRigidBody
	{
		b2Body* m_body = nullptr;

		b2PolygonShape m_bodyShape;

		b2FixtureDef m_fixtureDefinition;
	};


	struct STransform
	{
		Vec2 m_position = {0.0f, 0.0f};

		f32 m_rotation = 0.0f;

		Vec2 m_scale = { 0.0f, 0.0f };

		Mat4 m_worldTransform = Mat4(1.0f);
	};


	struct SSprite
	{
		bool m_visible = false;

		bool m_internalVisible = false;

		u32 m_depth = 0;

		CRect m_rect;

		bool m_flipX = false;

		bool m_flipY = false;

		mint::fx::CColor m_color;

		Vec2 m_origin;
	};


	struct SAnimatedSprite
	{
		f32 m_frameCounter;

		u32 m_animationSpeed;
	};


	struct SScript
	{
		ScriptHandle m_scriptHandle;
	};

}



#endif