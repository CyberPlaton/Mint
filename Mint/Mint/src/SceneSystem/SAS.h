#ifndef _MINT_SAS_H_
#define _MINT_SAS_H_


#include "THST/QuadTree.h"
#include "Utility/STL/Map.h"
#include "Common/Rectangle.h"
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



	class CSAS
	{
	public:
		STATIC_GET(CSAS, s_CSAS);

		bool initialize();

		void terminate();

		void reset();

		void run_sas_thread();

		bool is_running();

		void set_should_update(bool value);



		void submit_scene_dynamic_entities(Vector< entt::entity >& entities);

		void submit_scene_static_entities(Vector< entt::entity >& entities);

		Vector< entt::entity > retrieve_visible_entities();


	private:
		static CSAS* s_CSAS;

		bool m_internalLoop;

		bool m_running;

		bool m_update;

		MINT_CRITICAL_SECTION(m_criticalSection);


		detail::SQuadTreeContainer m_dqtree;

		detail::SQuadTreeContainer m_sqtree;

		using EntityMatrix = Vector< Vector< entt::entity > >;

		EntityMatrix m_layeredEntities;

		std::array< Vector< entt::entity >, MINT_SAS_OUT_QUEUE_COUNT_MAX > m_outQueue;

		u32 m_currentOut;


	private:
		void _run();

		void _internal_run();

		void _internal_computation();

		void _set_is_running(bool value);

		bool _should_update();

		void _wait_for_termination();

	};


}


#endif