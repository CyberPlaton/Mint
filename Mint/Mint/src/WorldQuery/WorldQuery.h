#ifndef _MINT_WORLD_QUERY_H_
#define _MINT_WORLD_QUERY_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Utility/Profiling/FunctionProfiler.h"
#include "Common/WorldQueryFilter.h"
#include "Common/WorldQueryType.h"
#include "Utility/STL/BinarySearchTree.h"


namespace mint
{
	class CWorldQueryDebugRender;


	class CWorldQuery : protected b2DynamicTree
	{

		friend class CWorldQueryDebugRender;

	public:
		STATIC_GET(CWorldQuery, s_CWorldQuery);

		bool initialize();

		void terminate();

		bool register_entity_proxy(entt::entity entity, const mint::CRect& destination_rect);

		bool is_entity_registered(entt::entity entity);

		void remove_entity_proxy(entt::entity entity);

		bool update_entity_proxy(entt::entity entity, const b2AABB& aabb, const Vec2& displacement);


		u32 get_entity_count_at_point_in_radius(const CRect& rect);

		template< class FilterType >
		u32 get_entity_count_at_point_in_radius(const CRect& rect, FilterType* filter);


		Vector< SWorldQueryProxy > get_entities_at_point_in_radius(const CRect& rect);

		template< class FilterType >
		Vector< SWorldQueryProxy > get_entities_at_point_in_radius(const CRect& rect, FilterType* filter);




		bool QueryCallback(s32 proxyId);
	
		f32 RayCastCallback(const b2RayCastInput& input, s32 proxyId);

	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		u32 m_masterQueryKey = 0;
		WorldQueryType m_masterQueryType;
		
		bool m_masterQueryHasFilter = false;
		CWorldQueryFilter* m_masterQueryFilter = nullptr;

		CBinarySearchTree< SWorldQueryProxy > m_registeredProxies;

		std::unordered_map< s32, u64 > m_registeredProxyIds;


		u64 m_queryResultEntityCount = 0;
		Vector< SWorldQueryProxy > m_queryResultEntityArray;
	};


	template< class FilterType >
	Vector< SWorldQueryProxy > mint::CWorldQuery::get_entities_at_point_in_radius(const CRect& rect, FilterType* filter)
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
	u32 mint::CWorldQuery::get_entity_count_at_point_in_radius(const CRect& rect, FilterType* filter)
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