#include "WorldQuery.h"


namespace mint
{

	bool CWorldQuery::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		m_registeredProxies.initialize(MINT_ENTITY_COUNT_MAX);

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

		if (m_registeredProxies.lookup(h)) return true;

		b2AABB aabb = mint::algorithm::compute_aabb(destination_rect);

		auto proxy = m_registeredProxies.add_node(h);

		proxy->m_entity = entity;
		proxy->m_b2ProxyId = CreateProxy(aabb, (void*)proxy);
		proxy->m_aabb = aabb;

		m_registeredProxyIds[proxy->m_b2ProxyId] = h;


		return true;
	}

	bool CWorldQuery::is_entity_registered(entt::entity entity)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::is_entity_registered");

		auto h = SCAST(u64, entity);

		return m_registeredProxies.lookup(h);
	}

	void CWorldQuery::remove_entity_proxy(entt::entity entity)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::remove_entity_proxy");

		auto h = SCAST(u64, entity);

		if (!m_registeredProxies.lookup(h)) return;

		auto proxy = m_registeredProxies.get(h);

		DestroyProxy(proxy->m_b2ProxyId);

		m_registeredProxies.remove_node(h);
	}

	bool CWorldQuery::update_entity_proxy(entt::entity entity, const b2AABB& aabb, const Vec2& displacement)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::update_entity_proxy");

		auto h = SCAST(u64, entity);

		if (!m_registeredProxies.lookup(h)) return false;

		auto proxy = m_registeredProxies.get(h);

		if (MoveProxy(proxy->m_b2ProxyId, aabb, { displacement.x, displacement.y }))
		{
			proxy->m_aabb = aabb;
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
		auto proxy = m_registeredProxies.get(m_registeredProxyIds[proxyId]);

		if (m_masterQueryKey == proxy->m_queryKey) return true;

		proxy->m_queryKey = m_masterQueryKey;


		// Check whether there is a filter and we should apply it.
		if(m_masterQueryHasFilter && !m_masterQueryFilter->does_proxy_pass_filter(*proxy))
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
			// Create proxy copy for output.
			SWorldQueryProxy pcopy;

			pcopy.m_entity = proxy->m_entity;
			pcopy.m_aabb = proxy->m_aabb;
			pcopy.m_b2ProxyId = proxy->m_b2ProxyId;
			pcopy.m_queryKey = proxy->m_queryKey;

			mint::algorithm::vector_push_back(m_queryResultEntityArray, pcopy); break;
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