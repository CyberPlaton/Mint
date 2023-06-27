#include "WorldQueryProxy.h"

namespace mint::world
{
	SWorldQueryProxy::SWorldQueryProxy() :
		m_entity(entt::null), m_b2ProxyId(-1), m_queryKey(-1)
	{
		m_aabb.lowerBound = { 0.0f, 0.0f };
		m_aabb.upperBound = { 0.0f, 0.0f };
	}

	SWorldQueryProxy::SWorldQueryProxy(const SWorldQueryProxy& rh)
	{
		m_entity = rh.m_entity;
		m_aabb = rh.m_aabb;
		m_b2ProxyId = rh.m_b2ProxyId;
		m_queryKey = rh.m_queryKey;
	}
}