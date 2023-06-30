#ifndef _MINT_WORLD_QUERY_H_
#define _MINT_WORLD_QUERY_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Utility/Profiling/FunctionProfiler.h"
#include "Common/WorldQueryFilter.h"
#include "Common/WorldQueryType.h"
#include "WorldQueryDatabase.h"


namespace mint { class CWorldQueryDebugRender; }

namespace mint::world
{

	class CWorldQuery : protected b2DynamicTree
	{

		friend class mint::CWorldQueryDebugRender;

	public:
		STATIC_GET(CWorldQuery, s_CWorldQuery);

		bool initialize();

		void terminate();

		bool register_entity_proxy(entt::entity entity, const mint::CRect& destination_rect, const String& label);

		bool is_entity_registered(entt::entity entity);

		void remove_entity_proxy(entt::entity entity);

		bool update_entity_proxy(entt::entity entity, const b2AABB& aabb, const Vec2& displacement);

		SWorldQueryProxy* get_entity_proxy(entt::entity entity);



		bool get_any_entity_at_point_in_radius(const CRect& rect);

		template< class FilterType >
		bool get_any_entity_at_point_in_radius(const CRect& rect, FilterType* filter);


		u32 get_entity_count_at_point_in_radius(const CRect& rect);

		template< class FilterType >
		u32 get_entity_count_at_point_in_radius(const CRect& rect, FilterType* filter);


		Vector< SWorldQueryProxy > get_entities_at_point_in_radius(const CRect& rect);

		template< class FilterType >
		Vector< SWorldQueryProxy > get_entities_at_point_in_radius(const CRect& rect, FilterType* filter);


		u32 get_entity_count_by_ray_intersection(const Vec2& start, const Vec2& end);

		template< class FilterType >
		u32 get_entity_count_by_ray_intersection(const Vec2& start, const Vec2& end, FilterType* filter);


		Vector< SWorldQueryProxy > get_entities_by_ray_intersection(const Vec2& start, const Vec2& end);

		template< class FilterType >
		Vector< SWorldQueryProxy > get_entities_by_ray_intersection(const Vec2& start, const Vec2& end, FilterType* filter);


		bool get_any_entity_ray_intersection(const Vec2& start, const Vec2& end);


		template< class FilterType >
		bool get_any_entity_ray_intersection(const Vec2& start, const Vec2& end, FilterType* filter);



		bool create_membership_node(const String& label);
		
		bool create_attitude_node(const String& label);
		
		bool create_classification_node(const String& label);

		bool remove_node(const String& label);

		bool add_membership(entt::entity entity, const String& label, f32 value);

		bool add_attitude(entt::entity entity, const String& label, f32 value);

		bool add_classification(entt::entity entity, const String& label, f32 value);







		bool QueryCallback(s32 proxyId);
	
		f32 RayCastCallback(const b2RayCastInput& input, s32 proxyId);

	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		CDatabase m_queryDatabase;

		u32 m_masterQueryKey = 0;
		WorldQueryType m_masterQueryType;
		
		bool m_masterQueryHasFilter = false;
		CWorldQueryFilter* m_masterQueryFilter = nullptr;

		CBinarySearchTree< SWorldQueryProxy > m_registeredProxies;

		std::unordered_map< s32, u64 > m_registeredProxyIds;


		u64 m_queryResultEntityCount = 0;
		Vector< SWorldQueryProxy > m_queryResultEntityArray;
		bool m_queryResultAnyOccurrence = false;


	private:
		bool _add_entity_relationship_edge(u64 from_node_id, const String& to_node_label, const String& edge_label, u64 edge_id, f32 edge_weight);
		bool _add_entity_entity_edge(entt::entity entity, entt::entity to_entity);

	};

	template< class FilterType >
	bool mint::world::CWorldQuery::get_any_entity_at_point_in_radius(const CRect& rect, FilterType* filter)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = true;
			m_masterQueryFilter = filter;

		);

		auto result = get_any_entity_at_point_in_radius(rect);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = false;
			m_masterQueryFilter = nullptr;

		);

		return result;
	}

	template< class FilterType >
	bool mint::world::CWorldQuery::get_any_entity_ray_intersection(const Vec2& start, const Vec2& end, FilterType* filter)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = true;
			m_masterQueryFilter = filter;

		);

		auto result = get_any_entity_ray_intersection(start, end);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = false;
			m_masterQueryFilter = nullptr;

		);

		return result;
	}

	template< class FilterType >
	u32 mint::world::CWorldQuery::get_entity_count_by_ray_intersection(const Vec2& start, const Vec2& end, FilterType* filter)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = true;
			m_masterQueryFilter = filter;

		);

		auto result = get_entity_count_by_ray_intersection(start, end);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = false;
			m_masterQueryFilter = nullptr;

		);

		return result;
	}

	template< class FilterType >
	Vector< SWorldQueryProxy >
		mint::world::CWorldQuery::get_entities_by_ray_intersection(const Vec2& start, const Vec2& end, FilterType* filter)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = true;
			m_masterQueryFilter = filter;

		);

		auto& result = get_entities_by_ray_intersection(start, end);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = false;
			m_masterQueryFilter = nullptr;

		);

		return result;
	}

	template< class FilterType >
	Vector< SWorldQueryProxy > mint::world::CWorldQuery::get_entities_at_point_in_radius(const CRect& rect, FilterType* filter)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = true;
			m_masterQueryFilter = filter;
		
		);

		auto& result = get_entities_at_point_in_radius(rect);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = false;
			m_masterQueryFilter = nullptr;

		);

		return result;
	}


	template< class FilterType >
	u32 mint::world::CWorldQuery::get_entity_count_at_point_in_radius(const CRect& rect, FilterType* filter)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = true;
			m_masterQueryFilter = filter;

		);

		auto& result = get_entity_count_at_point_in_radius(rect);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_masterQueryHasFilter = false;
			m_masterQueryFilter = nullptr;

		);

		return result;
	}


}


#endif