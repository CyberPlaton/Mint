#include "Particle.h"


namespace mint::fx
{

	SParticleDefinition::SParticleDefinition()
	{
	}

	SParticleDefinition::SParticleDefinition(const SParticleDefinition& rh)
	{
		m_positionStart = rh.m_positionStart;
		m_scaleStart = rh.m_scaleStart;
		m_scaleEnd = rh.m_scaleEnd;
		m_rotation = rh.m_rotation;
		m_colorStart = rh.m_colorStart;
		m_colorEnd = rh.m_colorEnd;
		m_lifespan = rh.m_lifespan;
		m_lifespanOffset = rh.m_lifespanOffset;
		m_scaleOffset = rh.m_scaleOffset;
		m_rotationOffset = rh.m_rotationOffset;
		m_colorOffset = rh.m_colorOffset;
	}

	SParticle::SParticle() :
		m_position({ 0.0f, 0.0f }), m_scale({ 0.0f, 0.0f }),
		m_rotation(0.0f), m_color(MINT_WHITE().as_cliteral()), 
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