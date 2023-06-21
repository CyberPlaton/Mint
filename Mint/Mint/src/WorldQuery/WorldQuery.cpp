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
		auto h = SCAST(u64, entity);

		return m_registeredProxies.find(h) != m_registeredProxies.end();
	}

	void CWorldQuery::remove_entity_proxy(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_registeredProxies.find(h) == m_registeredProxies.end()) return;

		DestroyProxy(m_registeredProxies[h].m_b2ProxyId);

		m_registeredProxies.erase(h);
	}

	bool CWorldQuery::update_entity_proxy(entt::entity entity, const b2AABB& aabb, const Vec2& displacement)
	{
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
		u32 result = 0;

		b2AABB aabb = mint::algorithm::compute_aabb(rect);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryKey = (m_masterQueryKey + 1) % MINT_QUERIES_COUNT_MAX;

			Query(this, aabb);

			result = m_queryResultEntityCount;

			m_queryResultEntityCount = 0;
		);
		
		return result;
	}

// 	mint::f32 CWorldQuery::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, f32 fraction)
// 	{ 
// 		return 0.0f;
// 	}
// 
// 	bool CWorldQuery::ReportFixture(b2Fixture* fixture)
// 	{
// 		auto pfixture = reinterpret_cast<SWorldQueryProxy*>(fixture->GetUserData().pointer);
// 
// 		// Already checked this fixture.
// 		if (m_masterQueryKey == pfixture->m_queryKey) return true;
// 
// 		// Assign query key.
// 		pfixture->m_queryKey = m_masterQueryKey;
// 
// 		// Increment query result count.
// 		m_queryResults[m_masterQueryKey].m_count++;
// 
// 		return true;
// 	}

	bool CWorldQuery::QueryCallback(s32 proxyId)
	{
		auto& proxy = m_registeredProxies[m_registeredProxyIds[proxyId]];

		if (m_masterQueryKey == proxy.m_queryKey) return true;

		proxy.m_queryKey = m_masterQueryKey;

		m_queryResultEntityCount++;

		return true;
	}

	mint::f32 CWorldQuery::RayCastCallback(const b2RayCastInput& input, s32 proxyId)
	{
		return 0.0f;
	}

}