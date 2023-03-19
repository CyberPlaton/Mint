#ifndef _MINT_CUCA_H_
#define _MINT_CUCA_H_


#include "Components.h"

#include "SceneSystem/Common/IScene.h"


namespace mint::component
{
	class CUCA
	{
	public:

		static Vec2 transform_get_position(entt::entity entity);



		static Vec2 sprite_get_texture_dimension(entt::entity entity);
		static u32 sprite_get_depth(entt::entity entity);






	private:
		static MINT_CRITICAL_SECTION(m_dirtyFlagCriticalSection);
		static MINT_CRITICAL_SECTION(m_identifierCriticalSection);
		static MINT_CRITICAL_SECTION(m_hierarchyCriticalSection);
		static MINT_CRITICAL_SECTION(m_transformCriticalSection);
		static MINT_CRITICAL_SECTION(m_rigidBodyCriticalSection);
		static MINT_CRITICAL_SECTION(m_spriteCriticalSection);
		static MINT_CRITICAL_SECTION(m_animatedSpriteCriticalSection);
		static MINT_CRITICAL_SECTION(m_scriptCriticalSection);
		static MINT_CRITICAL_SECTION(m_dynamicGameobjectCriticalSection);

	};


}


using CUCA = mint::component::CUCA;


#endif