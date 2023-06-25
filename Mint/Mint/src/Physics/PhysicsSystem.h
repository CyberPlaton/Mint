#ifndef _MINT_PHYSYCS_SYSTEM_H_
#define _MINT_PHYSYCS_SYSTEM_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Common/Timestep.h"
#include "Components/Components.h"
#include "Components/CUCA.h"


namespace mint
{

	class CPhysicsSystem
	{
	public:
		struct SDescription
		{
			Vec2 m_gravity = { 0.0f, 0.0f };

			u64 m_velocityIterations = 0;

			u64 m_positionIterations = 0;

			f32 m_fixedTimestep = 0.0f;

			bool m_debugRender = false;

			b2Draw* m_debugRenderer = nullptr;

			bool m_drawShapes = false;
			bool m_drawJoints = false;
			bool m_drawAABBs = false;
			bool m_drawCenterOfMasses = false;
		};

	public:
		STATIC_GET(CPhysicsSystem, s_CPhysicsSystem)

		bool initialize(SDescription& desc);

		void terminate();

		void reset();
		
		void update(f32 dt);

		void set_physics_active(bool value);

		bool are_physics_active();



	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		b2World* m_world = nullptr;

		SDescription m_config;

		bool m_active = true;
	};
}


#endif