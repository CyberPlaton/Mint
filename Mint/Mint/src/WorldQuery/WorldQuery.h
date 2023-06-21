#ifndef _MINT_WORLD_QUERY_H_
#define _MINT_WORLD_QUERY_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Common/WorldQueryFilter.h"
#include "Common/WorldQueryResult.h"
#include "Utility/Profiling/FunctionProfiler.h"


namespace mint
{

	struct SWorldQueryProxy
	{
		entt::entity m_entity = entt::null;
		b2AABB m_aabb;
		s32 m_b2ProxyId = -1;
		s32 m_queryKey = -1;
	};

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




		/// @brief Raycast query callback. Called for each fixture intersecting with the raycast.
		/// Please note that there is no order in reporting fixtures!
		/// @param fixture Fixture intersecting with the raycast.
		/// @param point Point in world of the intersection.
		/// @param normal Normal vector of the intersection.
		/// @param fraction Fraction of the ray where the intersection takes place, i.e. 1.5f means the 1.5x length of the ray, and 0.5f means 0.5x length of the ray.
		/// @return -1.0f for ignoring the intersection,
		///			{0.0f ... 1.0f} for adjusting the ray length: 
		///				0.0f => set ray length to 0.0x, basically removing it.
		///				1.0f => keep full ray length.
		///				0.nf => set ray length to 0.nx. Return "fraction" variable to make the ray just long enough and continue raycasting.
// 		f32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, f32 fraction) override final;


		/// @brief AABB query callback. Called for each fixture in given query area.
		/// @param fixture Fixture inside the query area.
		/// @return True to keep querying, False to stop.
// 		bool ReportFixture(b2Fixture* fixture) override final;

		/// @brief 
		/// @param proxyId 
		/// @return 
		bool QueryCallback(s32 proxyId);
		
		/// @brief 
		/// @param input 
		/// @param proxyId 
		/// @return 
		f32 RayCastCallback(const b2RayCastInput& input, s32 proxyId);


	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		u32 m_masterQueryKey = 0;

		u64 m_queryResultEntityCount = 0;

		std::unordered_map< u64, SWorldQueryProxy > m_registeredProxies;

		std::unordered_map< s32, u64 > m_registeredProxyIds;

	};


}


#endif