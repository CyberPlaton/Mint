#ifndef _MINT_COMPONENTS_H_
#define _MINT_COMPONENTS_H_


#include "Common/Common.h"


namespace mint::component
{

	struct SDirty
	{
		bool m_isDirty = false;
	};


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
	};


	struct STransform
	{
		Vec2 m_position = {0.0f, 0.0f};

		f32 m_rotation = 0.0f;

		Vec2 m_scale = { 0.0f, 0.0f };
	};


	struct SSprite
	{
		bool m_visible = false;

		bool m_internalVisible = false;

		u32 m_depth = 0;

		ShaderProgramHandle m_shaderHandle;

		TextureHandle m_textureHandle;

		u32 m_textureWidth = 0;

		u32 m_textureHeight = 0;

		Vec2 m_uv;

		bool m_flipX = false;

		bool m_flipY = false;

		Vec4 m_color;

		Vec2 m_origin;
	};


	struct SAnimatedSprite
	{
		f32 m_frameCounter;

		u32 m_animationSpeed;
	};


	struct SScript
	{
		ScriptHandle m_scriptHandle = BGFX_INVALID_HANDLE;
	};

}



#endif