#include "WorldQuery.h"


namespace mint
{

	bool CWorldQuery::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}

	void CWorldQuery::terminate()
	{
		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	bool CWorldQuery::register_entity_proxy(entt::entity entity, const mint::CRect& destination_rect)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::register_entity_proxy");

		auto h = SCAST(u64, entity);

		if (m_registeredProxies.find(h) != m_registeredProxies.end()) return true;

		b2AABB aabb = mint::algorithm::compute_aabb(destination_rect);

		m_registeredProxies[h].m_entity = entity;
		m_registeredProxies[h].m_b2ProxyId = CreateProxy(aabb, &m_registeredProxies[h]);
		m_registeredProxies[h].m_aabb = aabb;

		m_registeredProxyIds[m_registeredProxies[h].m_b2ProxyId] = h;

		return true;
	}

	bool CWorldQuery::is_entity_registered(entt::entity entity)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::is_entity_registered");

		auto h = SCAST(u64, entity);

		return m_registeredProxies.find(h) != m_registeredProxies.end();
	}

	void CWorldQuery::remove_entity_proxy(entt::entity entity)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::remove_entity_proxy");

		auto h = SCAST(u64, entity);

		if (m_registeredProxies.find(h) == m_registeredProxies.end()) return;

		DestroyProxy(m_registeredProxies[h].m_b2ProxyId);

		m_registeredProxies.erase(h);
	}

	bool CWorldQuery::update_entity_proxy(entt::entity entity, const b2AABB& aabb, const Vec2& displacement)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::update_entity_proxy");

		auto h = SCAST(u64, entity);

		if (m_registeredProxies.find(h) == m_registeredProxies.end()) return false;

		if (MoveProxy(m_registeredProxies[h].m_b2ProxyId, aabb, { displacement.x, displacement.y }))
		{
			m_registeredProxies[h].m_aabb = aabb;
			return true;
		}

		return false;
	}

	
	mint::u32 CWorldQuery::get_entity_count_at_point_in_radius(const CRect& rect)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::get_entity_count_at_point_in_radius");

		u32 result = 0;

		b2AABB aabb = mint::algorithm::compute_aabb(rect);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryType = WorldQueryType_EntityCount;

			m_masterQueryKey = (m_masterQueryKey + 1) % MINT_QUERIES_COUNT_MAX;

			Query(this, aabb);

			result = m_queryResultEntityCount;

			m_queryResultEntityCount = 0;
			m_masterQueryType = WorldQueryType_None;
		);

 		return result;
	}


	mint::Vector< mint::SWorldQueryProxy > CWorldQuery::get_entities_at_point_in_radius(const CRect& rect)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::get_entities_at_point_in_radius");

		Vector< SWorldQueryProxy > result;

		b2AABB aabb = mint::algorithm::compute_aabb(rect);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryType = WorldQueryType_EntityArray;

			m_masterQueryKey = (m_masterQueryKey + 1) % MINT_QUERIES_COUNT_MAX;

			Query(this, aabb);

			result = m_queryResultEntityArray;

			m_queryResultEntityArray.clear();
			m_masterQueryType = WorldQueryType_None;
		);


		return result;
	}

	bool CWorldQuery::QueryCallback(s32 proxyId)
	{
		auto& proxy = m_registeredProxies[m_registeredProxyIds[proxyId]];

		if (m_masterQueryKey == proxy.m_queryKey) return true;

		proxy.m_queryKey = m_masterQueryKey;


		// Check whether there is a filter and we should apply it.
		if(m_masterQueryHasFilter && !m_masterQueryFilter->does_proxy_pass_filter(proxy))
		{
			return true;
		}


		// Determine what to do based on query type.
		switch (m_masterQueryType)
		{
		case WorldQueryType_EntityCount:
		{
			m_queryResultEntityCount++; break;
		}
		case WorldQueryType_EntityArray:
		{
			mint::algorithm::vector_push_back(m_queryResultEntityArray, proxy); break;
		}
		default: return false;
		}


		return true;
	}

	mint::f32 CWorldQuery::RayCastCallback(const b2RayCastInput& input, s32 proxyId)
	{
		return 0.0f;
	}

}