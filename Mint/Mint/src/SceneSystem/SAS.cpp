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
		_set_is_running(false);

		_wait_for_termination();
	}


	void CSAS::reset()
	{
		m_dqtree.m_qtree.clear();
		m_sqtree.m_qtree.clear();

		for(auto& queue: m_outQueue)
		{
			queue.clear();
		}
	}


	void CSAS::run_sas_thread()
	{
		_run();
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


		for (auto& entity : entities)
		{
			MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::SSprite >(entity) &&
						MINT_SCENE_REGISTRY().has_component< mint::component::SDynamicGameobject >(entity),
						"Submitting dynamic entities is allowed only for dynamic entities with a Sprite component!");


			
		}
	}


	void CSAS::submit_scene_static_entities(Vector< entt::entity >& entities)
	{
		f32 worldmin[2] = { 0 };
		f32 worldmax[2] = { 0 };

		Vector< detail::SQuadTreeNode > bboxes;

		CRect visible_area = MINT_ACTIVE_SCENE()->get_active_camera()->get_world_visible_area();

		worldmin[0] = visible_area.get_x();
		worldmin[1] = visible_area.get_y();
		worldmax[0] = visible_area.get_width();
		worldmax[1] = visible_area.get_height();


		for (auto& entity : entities)
		{
			MINT_ASSERT(MINT_SCENE_REGISTRY().has_component< mint::component::SSprite >(entity) &&
						!MINT_SCENE_REGISTRY().has_component< mint::component::SDynamicGameobject >(entity),
						"Submitting static entities is allowed only for static entities with a Sprite component!");

			detail::SQuadTreeNode node;
			node.m_entity = entity;

			Vec2 position = CUCA::transform_get_position(entity);
			Vec2 dimension = CUCA::sprite_get_texture_dimension(entity);

			f32 emin[2] = { 0 };
			f32 emax[2] = { 0 };

			emin[0] = position.x;
			emin[1] = position.y;

			emax[0] = position.x + dimension.x;
			emax[1] = position.y + dimension.y;

			spatial::BoundingBox< f32, 2 > bbox(emin, emax);

			node.m_boundingBox = bbox;


			if (emin[0] < worldmin[0]) worldmin[0] = emin[0];
			if (emin[1] < worldmin[1]) worldmin[1] = emin[1];
			if (emax[0] > worldmax[0]) worldmax[0] = emax[0];
			if (emax[1] > worldmax[1]) worldmax[1] = emax[1];


			bboxes.push_back(node);
		}


		spatial::BoundingBox< f32, 2 > bbox(worldmin, worldmax);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_dqtree.m_qtree.clear();

			m_dqtree.m_qtree.setBox(bbox.min, bbox.max);

			m_dqtree.m_qtree.insert(bboxes.begin(), bboxes.end());
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


		if(result.empty()) return;


		// Presort the entities in a painters algorithm manner. In that way we can
		// avoid sorting in the renderer and iterate through the vector of entities directly.
		u32 current_highest_layer = 0;
		for(auto& node: result)
		{
			auto layer = CUCA::sprite_get_depth(node.m_entity);

			m_layeredEntities[layer].push_back(node.m_entity);

			if (layer > current_highest_layer) current_highest_layer = layer;
		}

		Vector< entt::entity > frame_entities;
		for(u32 lyr = 0; lyr < current_highest_layer; lyr++)
		{
			mint::algorithm::vector_push_back(frame_entities, m_layeredEntities[lyr]);
		}


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_currentOut = (m_currentOut + 1) % MINT_SAS_OUT_QUEUE_COUNT_MAX;

			m_outQueue[m_currentOut] = frame_entities;
		
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