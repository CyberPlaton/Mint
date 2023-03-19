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
			Vec2 m_gravity;

			u64 m_velocityIterations;

			u64 m_positionIterations;

			f32 m_fixedTimestep;

			bool m_debugRender;

			b2Draw* m_debugRenderer;

			bool m_drawShapes;
			bool m_drawJoints;
			bool m_drawAABBs;
			bool m_drawCenterOfMasses;
		};

	public:
		STATIC_GET(CPhysicsSystem, s_CPhysicsSystem);

		bool initialize(SDescription& desc);

		void terminate();

		void reset();
		
		void update(f32 dt = CTimestep::get_fps());



	private:
		static CPhysicsSystem* s_CPhysicsSystem;

		MINT_CRITICAL_SECTION(m_criticalSection);

		b2World* m_world;

		SDescription m_config;
	
	};
}


#endif