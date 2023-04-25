#include "ComponentsLuaBinding.h"

namespace mint::luaglue
{
	namespace script
	{

	}

	namespace behavior
	{
		
		bool identifier(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginClass< entt::entity >("Entity")
				.endClass();

			luabridge::getGlobalNamespace(state)
				.beginNamespace("Identifier")
					.addFunction("GetDebugName", &CUCA::SIdentifier_GetDebugName)
					.addFunction("SetDebugName", &CUCA::SIdentifier_SetDebugName)
					.addFunction("GetUUID", &CUCA::SIdentifier_GetUUID)
					.addFunction("GetIdentifier", &CUCA::SIdentifier_GetIdentifier)
 				.endNamespace();
			
			return true;
		}


		bool scene_hierarchy(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("Hierarchy")
					.addFunction("HasParent", &CUCA::SSceneHierarchy_HasParent)
					.addFunction("HasChildren", &CUCA::SSceneHierarchy_HasChildren)
					.addFunction("GetParent", &CUCA::SSceneHierarchy_GetParentEntity)
					.addFunction("SetParent", &CUCA::SSceneHierarchy_SetParentEntity)
					.addFunction("GetChildren", &CUCA::SSceneHierarchy_GetChildEntities)
					.addFunction("AddChild", &CUCA::SSceneHierarchy_AddChildEntity)
				.endNamespace();

			return true;
		}


		bool transform(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("Transform")
				.addFunction("SetTranslationLocal", &CUCA::STransform_SetTranslationLocal)
				.addFunction("SetRotationLocal", &CUCA::STransform_SetRotationLocal)

				.addFunction("Translate", &CUCA::STransform_Translate)
				.addFunction("SetTranslation", &CUCA::STransform_SetTranslation)

				.addFunction("Rotate", &CUCA::STransform_Rotate)
				.addFunction("SetRotation", &CUCA::STransform_SetRotation)

				.addFunction("GetPosition", &CUCA::STransform_GetPosition)
				.addFunction("GetRotation", &CUCA::STransform_GetRotation)

				.endNamespace();

			return true;
		}


		bool sprite(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("Sprite")
				.addFunction("IsVisible", &CUCA::SSprite_GetVisible)
				.addFunction("SetVisible", &CUCA::SSprite_SetVisible)
				.addFunction("__IsVisible__", &CUCA::SSprite_GetInternalVisible)

				.addFunction("GetDepth", &CUCA::SSprite_GetDepth)
				.addFunction("SetDepth", &CUCA::SSprite_SetDepth)

				.addFunction("GetScale", &CUCA::SSprite_GetScale)
				.addFunction("SetScale", &CUCA::SSprite_SetScale)

				.addFunction("GetSize", &CUCA::SSprite_GetSpriteDimension)
				.addFunction("GetSourceTextureSize", &CUCA::SSprite_GetTextureDimension)

				.addFunction("GetOrigin", &CUCA::SSprite_GetTextureOrigin)
				.addFunction("SetOrigin", &CUCA::SSprite_SetTextureOrigin)

				.addFunction("GetColor", &CUCA::SSprite_GetColorTint)
				.addFunction("SetColor", &CUCA::SSprite_SetColorTint)

				.addFunction("UsingShader", &CUCA::SSprite_GetUseShader)
				.addFunction("SetUsingShader", &CUCA::SSprite_SetUseShader)

				.addFunction("GetSourceRect", &CUCA::SSprite_GetSourceRectangle)
				.addFunction("SetSourceRect", &CUCA::SSprite_SetSourceRectangle)

				.addFunction("IsFlippedVertical", &CUCA::SSprite_GetFlipTextureVertical)
				.addFunction("IsFlippedHorizontal", &CUCA::SSprite_GetFlipTextureHorizontal)
				.addFunction("SetFlippedVertical", &CUCA::SSprite_FlipTextureVertical)
				.addFunction("SetFlippedHorizontal", &CUCA::SSprite_FlipTextureHorizontal)

#ifndef MINT_DISTR
				.addFunction("__RenderOutline__", &CUCA::SSprite_SetRenderOutline)
				.addFunction("__RenderDestRect__", &CUCA::SSprite_SetRenderDestinationRect)
				.addFunction("__RenderOrigin__", &CUCA::SSprite_SetRenderTextureOrigin)
#endif

				.endNamespace();

			return true;
		}


		bool animated_sprite(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("Animation")
				.addFunction("GetAnimationSpeed", &CUCA::SAnimatedSprite_GetAnimationSpeed)
				.addFunction("SetAnimationSpeed", &CUCA::SAnimatedSprite_SetAnimationSpeed)

				.addFunction("GetFrameCount", &CUCA::SAnimatedSprite_GetFrameCount)

				.addFunction("GetAnimatorIdentifier", &CUCA::SAnimatedSprite_GetAnimatorIdentifier)

				.endNamespace();

			return true;
		}


	}
}