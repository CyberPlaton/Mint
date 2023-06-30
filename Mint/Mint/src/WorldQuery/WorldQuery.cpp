#include "WorldQuery.h"


namespace mint::world
{

	bool CWorldQuery::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		bool result = true;

		result &= m_registeredProxies.initialize(MINT_ENTITY_COUNT_MAX);

#if MINT_USE_WORLD_QUERY_DATABASE
		result &= m_queryDatabase.initialize(MINT_ENTITY_COUNT_MAX);
#else
		MINT_LOG_WARN("[{:.4f}][CWorldQuery::initialize] Not using World Query Database!", MINT_APP_TIME);
#endif

		return result;
	}

	void CWorldQuery::terminate()
	{
#if MINT_USE_WORLD_QUERY_DATABASE
		m_queryDatabase.terminate();
#else
#endif

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	bool CWorldQuery::register_entity_proxy(entt::entity entity, const mint::CRect& destination_rect, const String& label)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::register_entity_proxy");

		auto h = SCAST(u64, entity);

		// Try registering in the Dynamic Tree.
		if (m_registeredProxies.lookup(h)) return true;

		b2AABB aabb = mint::algorithm::compute_aabb(destination_rect);

		auto proxy = m_registeredProxies.add_node(h);

		proxy->m_entity = entity;
		proxy->m_b2ProxyId = CreateProxy(aabb, (void*)proxy);
		proxy->m_aabb = aabb;

		m_registeredProxyIds[proxy->m_b2ProxyId] = h;

		// Try registering in the Database.
		if (!m_queryDatabase.create_node(h, label, NodeType_Entity, entity)) return false;

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

	
	bool CWorldQuery::get_any_entity_at_point_in_radius(const CRect& rect)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::get_any_entity_at_point_in_radius");

		bool result = false;

		b2AABB aabb = mint::algorithm::compute_aabb(rect);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryType = WorldQueryType_AnyOccurrence;

			m_masterQueryKey = (m_masterQueryKey + 1) % MINT_QUERIES_COUNT_MAX;

			Query(this, aabb);

			result = m_queryResultAnyOccurrence;

			m_queryResultAnyOccurrence = false;
			m_masterQueryType = WorldQueryType_None;

		);

		return result;
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


	mint::Vector< mint::world::SWorldQueryProxy > CWorldQuery::get_entities_at_point_in_radius(const CRect& rect)
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

	glm::u32 CWorldQuery::get_entity_count_by_ray_intersection(const Vec2& start, const Vec2& end)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::get_entity_count_by_ray_intersection");

		u32 result = 0;

		b2RayCastInput input_ray;
		input_ray.p1 = { start.x, start.y };
		input_ray.p2 = { end.x, end.y };
		input_ray.maxFraction = 1.0f;


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryType = WorldQueryType_EntityCount;

			m_masterQueryKey = (m_masterQueryKey + 1) % MINT_QUERIES_COUNT_MAX;

			RayCast(this, input_ray);

			result = m_queryResultEntityCount;

			m_queryResultEntityCount = 0;
			m_masterQueryType = WorldQueryType_None;

		);


		return result;
	}

	mint::Vector< mint::world::SWorldQueryProxy > CWorldQuery::get_entities_by_ray_intersection(const Vec2& start, const Vec2& end)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::get_entities_by_ray_intersection");

		Vector< SWorldQueryProxy > result;

		b2RayCastInput input_ray;
		input_ray.p1 = { start.x, start.y };
		input_ray.p2 = { end.x, end.y };
		input_ray.maxFraction = 1.0f;


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryType = WorldQueryType_EntityArray;

			m_masterQueryKey = (m_masterQueryKey + 1) % MINT_QUERIES_COUNT_MAX;

			RayCast(this, input_ray);

			result = m_queryResultEntityArray;

			m_queryResultEntityArray.clear();
			m_masterQueryType = WorldQueryType_None;

		);


		return result;
	}

	bool CWorldQuery::get_any_entity_ray_intersection(const Vec2& start, const Vec2& end)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::get_any_entity_ray_intersection");

		bool result = false;

		b2RayCastInput input_ray;
		input_ray.p1 = { start.x, start.y };
		input_ray.p2 = { end.x, end.y };
		input_ray.maxFraction = 1.0f;

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryType = WorldQueryType_AnyOccurrence;

			m_masterQueryKey = (m_masterQueryKey + 1) % MINT_QUERIES_COUNT_MAX;

			RayCast(this, input_ray);

			result = m_queryResultAnyOccurrence;

			m_queryResultAnyOccurrence = false;
			m_masterQueryType = WorldQueryType_None;

		);


		return result;
	}

	bool CWorldQuery::QueryCallback(s32 proxyId)
	{
		bool aabb_result = true;

		auto proxy = m_registeredProxies.get(m_registeredProxyIds[proxyId]);


		// Determine return value based on the aabb query mode.
		switch (m_masterQueryType)
		{
		case WorldQueryType_EntityCount: { aabb_result = true; break; } // Find all entities the aabb intersects with.
		case WorldQueryType_EntityArray: { aabb_result = true; break; }
		case WorldQueryType_AnyOccurrence: { aabb_result = false; break; }
		default: return false; // Stop the query.
		}


		if (m_masterQueryKey == proxy->m_queryKey) return aabb_result;

		proxy->m_queryKey = m_masterQueryKey;


		// Check whether there is a filter and we should apply it.
		if(m_masterQueryHasFilter && !m_masterQueryFilter->does_proxy_pass_filter(*proxy))
		{
			return aabb_result;
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
		case WorldQueryType_AnyOccurrence:
		{
			m_queryResultAnyOccurrence = true;
			break;
		}
		default: return false;
		}


		return aabb_result;
	}

	mint::f32 CWorldQuery::RayCastCallback(const b2RayCastInput& input, s32 proxyId) // TODO
	{
		f32 raycast_result = 0.0f;

		auto proxy = m_registeredProxies.get(m_registeredProxyIds[proxyId]);


		// Determine return value based on the raycast query mode.
		switch (m_masterQueryType)
		{
		case WorldQueryType_EntityCount: { raycast_result = 1.0f; break; } // Find all entities the ray intersects with.
		case WorldQueryType_EntityArray: { raycast_result = 1.0f; break; }
		case WorldQueryType_AnyOccurrence: {raycast_result = 0.0f; break; }
		default: return 0.0f; // Stop the query.
		}


		// Fast exit if proxy was already processed.
		if (m_masterQueryKey == proxy->m_queryKey) return raycast_result;

		proxy->m_queryKey = m_masterQueryKey;


		// Check whether there is a filter and we should apply it.
		if (m_masterQueryHasFilter && !m_masterQueryFilter->does_proxy_pass_filter(*proxy))
		{
			return raycast_result;
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
		case WorldQueryType_AnyOccurrence:
		{
			m_queryResultAnyOccurrence = true;
			break;
		}
		default: return 0.0f; // Stop the query.
		}


		return raycast_result;
	}

	mint::world::SWorldQueryProxy* CWorldQuery::get_entity_proxy(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_registeredProxies.lookup(h))
		{
			auto proxy = m_registeredProxies.get(h);

			return proxy;
		}
	}

	bool CWorldQuery::create_membership_node(const String& label)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::create_membership_node");

		bool result = m_queryDatabase.create_node(mint::algorithm::djb_hash(label), label, NodeType_Membership);

		return result;
	}

	bool CWorldQuery::create_attitude_node(const String& label)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::create_attitude_node");

		bool result = m_queryDatabase.create_node(mint::algorithm::djb_hash(label), label, NodeType_Attitude);

		return result;
	}

	bool CWorldQuery::create_classification_node(const String& label)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::create_classification_node");

		bool result = m_queryDatabase.create_node(mint::algorithm::djb_hash(label), label, NodeType_Classification);

		return result;
	}

	bool CWorldQuery::remove_node(const String& label)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::remove_node");

		bool result = m_queryDatabase.remove_node(mint::algorithm::djb_hash(label));

		return result;
	}

	bool CWorldQuery::add_membership(entt::entity entity, const String& label, f32 value)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::add_membership");

		auto h = SCAST(u64, entity);

		if (!m_registeredProxies.lookup(h)) return false;

		auto hh = mint::algorithm::djb_hash(label);

		bool result = _add_entity_relationship_edge(h, label, label, hh, value);

		return result;
	}

	bool CWorldQuery::add_attitude(entt::entity entity, const String& label, f32 value)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::add_attitude");

		return add_membership(entity, label, value);
	}

	bool CWorldQuery::add_classification(entt::entity entity, const String& label, f32 value)
	{
		MINT_PROFILE_SCOPE("Engine::WorldQuery", "CWorldQuery::add_classification");

		return add_membership(entity, label, value);
	}

	bool CWorldQuery::_add_entity_relationship_edge(u64 from_node_id, const String& to_node_label, const String& edge_label, u64 edge_id, f32 edge_weight)
	{
		bool result = m_queryDatabase.create_edge(from_node_id, mint::algorithm::djb_hash(to_node_label), edge_id, edge_label, edge_weight);

		return result;
	}

}