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
	MINT_CRITICAL_SECTION(CUCA::m_dirtyFlagCriticalSection);
	MINT_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);
	


	mint::Vec2 CUCA::transform_get_position(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_transformCriticalSection,

			Vec2 v = MINT_SCENE_REGISTRY().get_component< STransform >(entity).m_position;

		);

		return v;
	}


	mint::Vec2 CUCA::sprite_get_texture_dimension(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto w = MINT_SCENE_REGISTRY().get_component< SSprite >(entity).m_textureWidth;
			auto h = MINT_SCENE_REGISTRY().get_component< SSprite >(entity).m_textureHeight;

		);

		return { w, h };
	}


	mint::u32 CUCA::sprite_get_depth(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_spriteCriticalSection,

			auto value = MINT_SCENE_REGISTRY().get_component< SSprite >(entity).m_depth;
		
		);

		return value;
	}


}