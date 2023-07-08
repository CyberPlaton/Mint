#include "Particle.h"


namespace mint::fx
{

	SParticleDefinition::SParticleDefinition()
	{
	}

	SParticleDefinition::SParticleDefinition(const SParticleDefinition& rh)
	{
		m_tangentialVelocity = rh.m_tangentialVelocity;
		m_tangentialVelocityFalloff = rh.m_tangentialVelocityFalloff;
		m_tangentialVelocityOffset = rh.m_tangentialVelocityOffset;

		m_angularVelocity = rh.m_angularVelocity;
		m_angularVelocityFalloff = rh.m_angularVelocityFalloff;
		m_angularVelocityOffset = rh.m_angularVelocityOffset;

		m_startingDirection = rh.m_startingDirection;
		m_startingDirectionOffset = rh.m_startingDirectionOffset;

		m_positionStart = rh.m_positionStart;
		m_positionStartOffset = rh.m_positionStartOffset;

		m_scaleStart = rh.m_scaleStart;
		m_scaleEnd = rh.m_scaleEnd;
		m_scaleOffset = rh.m_scaleOffset;

		m_rotation = rh.m_rotation;
		m_rotationOffset = rh.m_rotationOffset;

		m_colorStart = rh.m_colorStart;
		m_colorHalf = rh.m_colorHalf;
		m_colorEnd = rh.m_colorEnd;
		m_colorOffset = rh.m_colorOffset;

		m_lifespan = rh.m_lifespan;
		m_lifespanOffset = rh.m_lifespanOffset;

		m_material = rh.m_material;
		m_emissionRate = rh.m_emissionRate;

		m_mode = rh.m_mode;
		m_gravity = rh.m_gravity;
		m_tangentialVelocityEase = rh.m_tangentialVelocityEase;
		m_angularVelocityEase = rh.m_angularVelocityEase;
		m_scaleEase = rh.m_scaleEase;
		m_rotationEase = rh.m_rotationEase;
		m_colorEase = rh.m_colorEase;
	}

	void SParticleDefinition::copy_from(const SParticleDefinition& rh)
	{
		m_tangentialVelocity = rh.m_tangentialVelocity;
		m_tangentialVelocityFalloff = rh.m_tangentialVelocityFalloff;
		m_tangentialVelocityOffset = rh.m_tangentialVelocityOffset;

		m_angularVelocity = rh.m_angularVelocity;
		m_angularVelocityFalloff = rh.m_angularVelocityFalloff;
		m_angularVelocityOffset = rh.m_angularVelocityOffset;

		m_startingDirection = rh.m_startingDirection;
		m_startingDirectionOffset = rh.m_startingDirectionOffset;

		m_positionStart = rh.m_positionStart;
		m_positionStartOffset = rh.m_positionStartOffset;

		m_scaleStart = rh.m_scaleStart;
		m_scaleEnd = rh.m_scaleEnd;
		m_scaleOffset = rh.m_scaleOffset;

		m_rotation = rh.m_rotation;
		m_rotationOffset = rh.m_rotationOffset;

		m_colorStart = rh.m_colorStart;
		m_colorHalf = rh.m_colorHalf;
		m_colorEnd = rh.m_colorEnd;
		m_colorOffset = rh.m_colorOffset;

		m_lifespan = rh.m_lifespan;
		m_lifespanOffset = rh.m_lifespanOffset;

		m_material = rh.m_material;
		m_emissionRate = rh.m_emissionRate;

		m_mode = rh.m_mode;
		m_gravity = rh.m_gravity;
		m_tangentialVelocityEase = rh.m_tangentialVelocityEase;
		m_angularVelocityEase = rh.m_angularVelocityEase;
		m_scaleEase = rh.m_scaleEase;
		m_rotationEase = rh.m_rotationEase;
		m_colorEase = rh.m_colorEase;
	}

	SParticle::SParticle() :
		m_position({ 0.0f, 0.0f }), m_scale({ 0.0f, 0.0f }),
		m_rotation(0.0f), m_color(MINT_WHITE()), 
		m_life(0.0f), m_active(false)
	{
	}

	SParticle::SParticle(const SParticle& rh)
	{
		m_position = rh.m_position;
		m_scale = rh.m_scale;
		m_rotation = rh.m_rotation;
		m_color = rh.m_color;
		m_life = rh.m_life;
		m_active = false;
	}

	mint::fx::SParticle& SParticle::operator=(const SParticle& rh)
	{
		m_position = rh.m_position;
		m_scale = rh.m_scale;
		m_rotation = rh.m_rotation;
		m_color = rh.m_color;
		m_life = rh.m_life;
		m_active = false;

		return *this;
	}

	void SParticle::reset()
	{
		m_tangentialVelocity = 0.0f;
		m_angularVelocity = 0.0f;
		m_rotation = 0.0f;
		m_life = 0.0f;
		m_lookingDirection = { 0.0f, 0.0f };
		m_position = { 0.0f, 0.0f };
		m_scale = { 0.0f, 0.0f };
		m_color = { 0, 0, 0, 0 };
		m_active = false;
	}

}