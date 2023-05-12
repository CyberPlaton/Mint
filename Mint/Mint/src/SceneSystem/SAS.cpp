#include "SAS.h"


namespace mint
{
	mint::CSAS* CSAS::s_CSAS = nullptr;


	bool CSAS::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		m_internalLoop = false;
		m_running = false;
		m_update = false;

		m_layeredEntities.resize(MINT_SAS_RENDERING_LAYERS_MAX);

		return true;
	}


	void CSAS::terminate()
	{
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CSAS::reset()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_dqtree.m_qtree.clear();
			m_sqtree.m_qtree.clear();

			for (auto& queue : m_outQueue)
			{
				queue.clear();
			}

			for (u32 lyr = 0; lyr < MINT_SAS_RENDERING_LAYERS_MAX; lyr++)
			{
				m_layeredEntities[lyr].clear();
			}
		);

	}

	void CSAS::run_sas_thread()
	{
		_run();
	}


	void CSAS::stop_sas_thread()
	{
		set_should_update(false);

		_set_is_running(false);

		_wait_for_termination();
	}


	void CSAS::_wait_for_termination()
	{
		while (true)
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				const bool running = m_internalLoop;

			);

			if (!running) return;

			std::this_thread::sleep_for(std::chrono::duration< s32 >::zero());
		}
	}


	bool CSAS::is_running()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool v = m_running;

		);

		return v;
	}


	void CSAS::set_should_update(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_update = value;

		);
	}


	void CSAS::submit_scene_dynamic_entities(Vector< entt::entity >& entities)
	{
		f32 worldmin[2] = { 0 };
		f32 worldmax[2] = { 0 };

		Vector< detail::SQuadTreeNode > bboxes;

		// Find minimal and maximal position for all entities, this will be the minimum / maximum of the Quad Tree.
		f32 min[2] = { 0 };
		min[0] = 0;
		min[1] = 0;

		f32 max[2] = { 0 };
		max[0] = 0;
		max[1] = 0;

		// Viewport bounding box.
		f32 view_min[2] = { 0 };
		view_min[0] = 0;
		view_min[1] = 0;

		f32 view_max[2] = { 0 };
		view_max[0] = 0;
		view_max[1] = 0;

		auto area = MINT_ACTIVE_SCENE()->get_active_camera()->get_world_visible_area();
		view_min[0] = area.get_x();
		view_min[1] = area.get_y();
		view_max[0] = area.get_width();
		view_max[1] = area.get_height();


		for (auto& entity : entities)
		{
			MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::SSprite >(entity) &&
						MINT_SCENE_REGISTRY().has_component< mint::component::SDynamicGameobject >(entity),
						"Submitting dynamic entities is allowed only for dynamic entities with a Sprite component!");

			if (!CUCA::sprite_is_visible(entity) || !CUCA::sprite_is_internal_visible(entity)) continue;

			detail::SQuadTreeNode node;
			node.m_entity = entity;

			auto dest = CUCA::sprite_get_destination_rect(entity);
			
			f32 entity_min[2] = { 0 };
			entity_min[0] = dest.get_x();
			entity_min[1] = dest.get_y();

			f32 entity_max[2] = { 0 };
			entity_max[0] = dest.get_x() + dest.get_width();
			entity_max[1] = dest.get_y() + dest.get_height();

			spatial::BoundingBox< f32, 2 > bbox(entity_min, entity_max);
			node.m_boundingBox = bbox;

			// Find minimum and maximum point in world.
			if (entity_min[0] < min[0]) min[0] = entity_min[0];
			if (entity_min[1] < min[1]) min[1] = entity_min[1];
			if (entity_max[0] > max[0]) max[0] = entity_max[0];
			if (entity_max[1] > max[1]) max[1] = entity_max[1];

			mint::algorithm::vector_push_back(bboxes, node);
		}

		// Adjust the minimum and maximum according to viewport and entity positioning.
		min[0] = glm::min(min[0], view_min[0]);
		min[1] = glm::min(min[1], view_min[1]);
		max[0] = glm::max(max[0], view_max[0]);
		max[1] = glm::max(max[1], view_max[1]);

		spatial::BoundingBox< f32, 2 > bbox(min, max);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_dqtree.m_qtree.clear();
			
			m_dqtree.m_qtree.setBox(bbox.min, bbox.max);

			m_dqtree.m_qtree.insert(bboxes.begin(), bboxes.end());
		);
	}


	void CSAS::submit_scene_static_entities(Vector< entt::entity >& entities)
	{
		f32 worldmin[2] = { 0 };
		f32 worldmax[2] = { 0 };

		Vector< detail::SQuadTreeNode > bboxes;

		// Find minimal and maximal position for all entities, this will be the minimum / maximum of the Quad Tree.
		f32 min[2] = { 0 };
		min[0] = 0;
		min[1] = 0;

		f32 max[2] = { 0 };
		max[0] = 0;
		max[1] = 0;

		// Viewport bounding box.
		f32 view_min[2] = { 0 };
		view_min[0] = 0;
		view_min[1] = 0;

		f32 view_max[2] = { 0 };
		view_max[0] = 0;
		view_max[1] = 0;

		auto area = MINT_ACTIVE_SCENE()->get_active_camera()->get_world_visible_area();
		view_min[0] = area.get_x();
		view_min[1] = area.get_y();
		view_max[0] = area.get_width();
		view_max[1] = area.get_height();


		for (auto& entity : entities)
		{
			MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::SSprite >(entity) &&
					   !MINT_SCENE_REGISTRY().has_component< mint::component::SDynamicGameobject >(entity),
					   "Submitting static entities is allowed only for static entities with a Sprite component!");

			if (!CUCA::sprite_is_visible(entity) || !CUCA::sprite_is_internal_visible(entity)) continue;


			detail::SQuadTreeNode node;
			node.m_entity = entity;

			auto dest = CUCA::sprite_get_destination_rect(entity);

			f32 entity_min[2] = { 0 };
			entity_min[0] = dest.get_x();
			entity_min[1] = dest.get_y();

			f32 entity_max[2] = { 0 };
			entity_max[0] = dest.get_x() + dest.get_width();
			entity_max[1] = dest.get_y() + dest.get_height();

			spatial::BoundingBox< f32, 2 > bbox(entity_min, entity_max);
			node.m_boundingBox = bbox;

			// Find minimum and maximum point in world.
			if (entity_min[0] < min[0]) min[0] = entity_min[0];
			if (entity_min[1] < min[1]) min[1] = entity_min[1];
			if (entity_max[0] > max[0]) max[0] = entity_max[0];
			if (entity_max[1] > max[1]) max[1] = entity_max[1];

			mint::algorithm::vector_push_back(bboxes, node);
		}

		// Adjust the minimum and maximum according to viewport and entity positioning.
		min[0] = glm::min(min[0], view_min[0]);
		min[1] = glm::min(min[1], view_min[1]);
		max[0] = glm::max(max[0], view_max[0]);
		max[1] = glm::max(max[1], view_max[1]);

		spatial::BoundingBox< f32, 2 > bbox(min, max);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_sqtree.m_qtree.clear();

			m_sqtree.m_qtree.setBox(bbox.min, bbox.max);

			m_sqtree.m_qtree.insert(bboxes.begin(), bboxes.end());
		);
	}

	mint::Vector< entt::entity > CSAS::retrieve_visible_entities()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto out = m_outQueue[m_currentOut];

			);

		return out;
	}


	void CSAS::_run()
	{
		std::thread thread(&CSAS::_internal_run, this);

		thread.detach();
	}


	void CSAS::_internal_run()
	{
		_set_is_running(true);

		while(is_running())
		{
			if(_should_update())
			{
				_internal_computation();
			}
			else
			{
				std::this_thread::yield();
			}
		}

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_internalLoop = false;

		);
	}


	void CSAS::_internal_computation()
	{
		set_should_update(false);

		f32 viewmin[2] = { 0 };
		f32 viewmax[2] = { 0 };

		CRect visible_area = MINT_ACTIVE_SCENE()->get_active_camera()->get_world_visible_area();

		viewmin[0] = visible_area.get_x();
		viewmin[1] = visible_area.get_y();
		viewmax[0] = visible_area.get_width();
		viewmax[1] = visible_area.get_height();


		spatial::BoundingBox< f32, 2 > bbox(viewmin, viewmax);
		Vector< detail::SQuadTreeNode > result;
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				m_sqtree.m_qtree.query(spatial::intersects<2>(bbox.min, bbox.max), std::back_inserter(result));

				m_dqtree.m_qtree.query(spatial::intersects<2>(bbox.min, bbox.max), std::back_inserter(result));

			);
		}


		u32 current_highest_layer = 0;
		Vector< entt::entity > frame_entities;

		if(!result.empty())
		{
			// Presort the entities in a painters algorithm manner. In that way we can
			// avoid sorting in the renderer and iterate through the vector of entities directly.
			for (auto& node : result)
			{
				auto layer = CUCA::sprite_get_depth(node.m_entity);

				m_layeredEntities[layer].push_back(node.m_entity);

				if (layer > current_highest_layer) current_highest_layer = layer;
			}

			for (u32 lyr = 0; lyr <= current_highest_layer; lyr++)
			{
				mint::algorithm::vector_push_back(frame_entities, m_layeredEntities[lyr]);
			}
		}


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_currentOut = (m_currentOut + 1) % MINT_SAS_OUT_QUEUE_COUNT_MAX;

			m_outQueue[m_currentOut] = frame_entities;

		);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			// Clean up rendering layers, inclusive the highest rendered layer.
			for (u32 lyr = 0; lyr <= current_highest_layer; lyr++)
			{
				m_layeredEntities[lyr].clear();
			}

		);
	}


	void CSAS::_set_is_running(bool value)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_running = value;

		);
	}


	bool CSAS::_should_update()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool v = m_update;

		);

		return v;
	}




	namespace detail
	{


		bool SQuadTreeNode::operator==(const SQuadTreeNode& rhs) const
		{
			return m_entity == rhs.m_entity;
		}


		SQuadTreeContainer::SQuadTreeContainer(spatial::BoundingBox< f32, 2 > bbox /*= spatial::BoundingBox< f32, 2 >(0.0f, 1.0f)*/) :
			m_qtree(bbox.min, bbox.max)
		{

		}


	}

}