#ifndef _MINT_WORLD_QUERY_PROXY_H_
#define _MINT_WORLD_QUERY_PROXY_H_


#include "Common/Common.h"


namespace mint
{

	struct SWorldQueryProxy
	{
		SWorldQueryProxy();
		SWorldQueryProxy(const SWorldQueryProxy& rh);
		SWorldQueryProxy& operator=(const SWorldQueryProxy& rh)
		{
			m_entity = rh.m_entity;
			m_aabb = rh.m_aabb;
			m_b2ProxyId = rh.m_b2ProxyId;
			m_queryKey = rh.m_queryKey;

			return *this;
		}


		entt::entity m_entity;
		b2AABB m_aabb;
		s32 m_b2ProxyId;
		s32 m_queryKey;
	};


}


#endif