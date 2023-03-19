#ifndef _MINT_SAS_H_
#define _MINT_SAS_H_


#include "THST/QuadTree.h"
#include "Utility/STL/Map.h"

#include "Utility/ServiceSystem/Service.h"

#include "Components/CUCA.h"

#include "Common/IScene.h"


namespace mint
{

	namespace detail
	{
		struct SQuadTreeNode
		{
			bool operator==(const SQuadTreeNode& rhs) const;


			spatial::BoundingBox< f32, 2 > m_boundingBox;
			entt::entity m_entity;
		};



		struct SQuadTreeIndexable
		{
			const f32* min(const SQuadTreeNode& value) const { return value.m_boundingBox.min; }
			const f32* max(const SQuadTreeNode& value) const { return value.m_boundingBox.max; }
		};



		struct SQuadTreeContainer
		{
			SQuadTreeContainer(spatial::BoundingBox< f32, 2 > bbox = spatial::BoundingBox< f32, 2 >(0.0f, 1.0f));


			spatial::QuadTree<f32, SQuadTreeNode, 16, SQuadTreeIndexable> m_qtree;
		};


	}


	class IMintEngine;

	class CSAS : public IService
	{
		friend class IMintEngine;
	public:
		STATIC_GET(CSAS, g_CSAS);

		bool initialize() override final;

		void terminate() override final;

		void reset() override final;

		String get_service_type() override final { return "SAS"; }



		void wait_for_termination();

		bool is_running();

		void set_should_update(bool value);



		void submit_scene_dynamic_entities(Vector< entt::entity >& entities);

		void submit_scene_static_entities(Vector< entt::entity >& entities);

		Vector< entt::entity > retrieve_visible_entities();


	private:
		bool m_internalLoop;

		bool m_running;

		bool m_update;

		MINT_CRITICAL_SECTION(m_criticalSection);


		detail::SQuadTreeContainer m_dqtree;

		detail::SQuadTreeContainer m_sqtree;


		Vector< Vector< entt::entity > > m_layeredEntities;

		std::array< Vector< entt::entity >, MINT_SAS_OUT_QUEUE_COUNT_MAX > m_outQueue;

		u32 m_currentOut;


	private:
		void _run();

		void _internal_run();

		void _internal_computation();

		void _set_is_running(bool value);

		bool _should_update();
	};


}


#endif