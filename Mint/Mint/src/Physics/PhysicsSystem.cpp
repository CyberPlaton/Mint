#include "PhysicsSystem.h"


namespace mint
{

	mint::CPhysicsSystem* CPhysicsSystem::s_CPhysicsSystem = nullptr;


	bool CPhysicsSystem::s_usePhysics = false;


	void CPhysicsSystem::set_use_physics(bool value)
	{
		s_usePhysics = value;
	}


	bool CPhysicsSystem::get_use_physics()
	{
		return s_usePhysics;
	}


	bool CPhysicsSystem::initialize(SDescription& desc)
	{
		b2Vec2 g(desc.m_gravity.x, desc.m_gravity.y);

		m_world = new b2World(g);
		m_world->SetGravity(g);
		m_world->SetAutoClearForces(true);

		m_config = desc;

		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}


	void CPhysicsSystem::terminate()
	{
		b2Body* body = m_world->GetBodyList();
		while( body )
		{
			m_world->DestroyBody(body);

			body = body->GetNext();
		}

		delete m_world; m_world = nullptr;

		delete m_config.m_debugRenderer; m_config.m_debugRenderer = nullptr;

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CPhysicsSystem::reset()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			b2Body* body = m_world->GetBodyList();
			while (body)
			{
				m_world->DestroyBody(body);

				body = body->GetNext();
			}

		);
	}


	void CPhysicsSystem::update(f32 dt /*= 0.0f*/)
	{
		m_world->Step(dt, m_config.m_velocityIterations, m_config.m_positionIterations);

		for(auto& entity: MINT_SCENE_REGISTRY().get_registry_view< mint::component::SRigidBody >())
		{
			const auto& rb = MINT_SCENE_REGISTRY().get_component< mint::component::SRigidBody >(entity);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				auto position = rb.m_body->GetPosition();

				auto&& aabb = rb.m_body->GetFixtureList()->GetAABB(0);

				const auto& extents = aabb.GetExtents();
				const auto& center = aabb.GetCenter();

				f32 x = center.x - extents.x;
				f32 y = center.y - extents.y;

				auto rotation = rb.m_body->GetAngle();
			);

			CUCA::transform_set_position(entity, { x, y });
			CUCA::transform_set_rotation(entity, mint::algorithm::radians_to_degree(rotation));
		}
	}


}