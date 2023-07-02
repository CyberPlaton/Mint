#ifndef _MINTFX_PARTICLE_H_
#define _MINTFX_PARTICLE_H_


#include "Common/Common.h"
#include "Graphics/Common/Color.h"


namespace mint::fx
{

	struct SParticleDefinition
	{
		SParticleDefinition();
		SParticleDefinition(const SParticleDefinition& rh);

		Vec2 m_velocity;
		Vec2 m_velocityOffset;

		Vec2 m_positionStart;
		Vec2 m_positionEnd;
		Vec2 m_positionOffset;

		Vec2 m_scaleStart;
		Vec2 m_scaleEnd;
		Vec2 m_scaleOffset;

		f32 m_rotation;
		f32 m_rotationOffset;

		CColor m_colorStart;
		CColor m_colorEnd;
		u8 m_colorOffset;

		f32 m_lifespan;
		f32 m_lifespanOffset;
	};


	struct SParticle
	{
		SParticle();
		SParticle(const SParticle& rh);
		SParticle& operator=(const SParticle& rh);

		Vec2 m_position;
		Vec2 m_scale;
		f32 m_rotation;
		raylib::Color m_color;
		f32 m_life;
		bool m_active;
	};

}

#endif