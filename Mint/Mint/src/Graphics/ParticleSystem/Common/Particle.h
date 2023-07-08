#ifndef _MINTFX_PARTICLE_H_
#define _MINTFX_PARTICLE_H_


#include "Common/Common.h"
#include "Graphics/Common/Color.h"
#include "Common/easing.h"


namespace mint::fx
{

	enum ParticleEmitterMode
	{
		ParticleEmitterMode_None = 0,
		ParticleEmitterMode_Gravity,
		ParticleEmitterMode_Free,
	};


	struct SParticleDefinition
	{
		SParticleDefinition();
		SParticleDefinition(const SParticleDefinition& rh);

		void copy_from(const SParticleDefinition& rh);


		f32 m_tangentialVelocity = 0.0f;
		Vec2 m_tangentialVelocityFalloff = { 0.0f, 0.0f };
		Vec2 m_tangentialVelocityOffset = { 0.0f, 0.0f };

		f32 m_angularVelocity = 0.0f;
		Vec2 m_angularVelocityFalloff = { 0.0f, 0.0f };
		Vec2 m_angularVelocityOffset = { 0.0f, 0.0f };
		
		Vec2 m_startingDirection = { 0.0f, 0.0f };
		Vec2 m_startingDirectionOffset = { 0.0f, 0.0f };

		Vec2 m_positionStart = { 0.0f, 0.0f };
		Vec2 m_positionStartOffset = { 0.0f, 0.0f };

		Vec2 m_scaleStart = { 0.0f, 0.0f };
		Vec2 m_scaleEnd = { 0.0f, 0.0f };
		Vec2 m_scaleOffset = { 0.0f, 0.0f };

		f32 m_rotation = 0.0f;
		Vec2 m_rotationOffset = { 0.0f, 0.0f };

		CColor m_colorStart = { 0, 0, 0, 0 };
		CColor m_colorHalf = { 0, 0, 0, 0 };
		CColor m_colorEnd = { 0, 0, 0, 0 };
		Vec2 m_colorOffset = { 0.0f, 0.0f };

		f32 m_lifespan = 0.0f;
		Vec2 m_lifespanOffset = { 0.0f, 0.0f };


		String m_material;
		f32 m_emissionRate = 0.0f;

		ParticleEmitterMode m_mode = ParticleEmitterMode_None;
		Vec2 m_gravity = { 0.0f, 0.0f };

		bx::Easing::Enum m_tangentialVelocityEase = bx::Easing::SmoothStep;
		bx::Easing::Enum m_angularVelocityEase = bx::Easing::SmoothStep;
		bx::Easing::Enum m_scaleEase = bx::Easing::SmoothStep;
		bx::Easing::Enum m_rotationEase = bx::Easing::SmoothStep;
		bx::Easing::Enum m_colorEase = bx::Easing::SmoothStep;
	};


	struct SParticle
	{
		SParticle();
		SParticle(const SParticle& rh);
		SParticle& operator=(const SParticle& rh);

		void reset();


		f32 m_tangentialVelocity = 0.0f;
		f32 m_angularVelocity = 0.0f;
		f32 m_rotation = 0.0f;
		f32 m_life = 0.0f;
		Vec2 m_lookingDirection = { 0.0f, 0.0f };
		Vec2 m_position = { 0.0f, 0.0f };
		Vec2 m_scale = { 0.0f, 0.0f };
		CColor m_color = {0, 0, 0, 0};
		bool m_active = false;
	};

}

#endif