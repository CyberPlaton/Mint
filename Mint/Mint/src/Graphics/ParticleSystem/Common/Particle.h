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


		f32 m_tangentialVelocity = 0.0f;
		f32 m_tangentialVelocityFalloff = 0.0f;
		Vec2 m_tangentialVelocityOffset = { 0.0f, 0.0f };

		f32 m_angularVelocity = 0.0f;
		f32 m_angularVelocityFalloff = 0.0f;
		Vec2 m_angularVelocityOffset = { 0.0f, 0.0f };
		
		Vec2 m_startingDirection = { 0.0f, 0.0f };
		Vec2 m_startingDirectionOffset = { 0.0f, 0.0f };

		Vec2 m_positionStart = { 0.0f, 0.0f };
		Vec2 m_positionStartOffset = { 0.0f, 0.0f };

		Vec2 m_scaleStart;
		Vec2 m_scaleEnd;
		Vec2 m_scaleOffset;

		f32 m_rotation;
		Vec2 m_rotationOffset;

		CColor m_colorStart;
		CColor m_colorEnd;
		Vec2 m_colorOffset;

		f32 m_lifespan;
		Vec2 m_lifespanOffset;
	};


	struct SParticle
	{
		SParticle();
		SParticle(const SParticle& rh);
		SParticle& operator=(const SParticle& rh);

		Vec2 m_lookingDirection = { 0.0f, 0.0f };
		f32 m_tangentialVelocity = 0.0f;
		f32 m_angularVelocity = 0.0f;
		Vec2 m_position;
		Vec2 m_scale;
		f32 m_rotation;
		raylib::Color m_color;
		f32 m_life;
		bool m_active;
	};

}

#endif