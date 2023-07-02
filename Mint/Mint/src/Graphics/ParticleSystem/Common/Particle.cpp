#include "Particle.h"


namespace mint::fx
{

	SParticleDefinition::SParticleDefinition() :
		m_positionStart({0.0f, 0.0f}), m_positionEnd({ 0.0f, 0.0f }),
		m_scaleStart({ 0.0f, 0.0f }), m_scaleEnd({ 0.0f, 0.0f }),
		m_velocity({ 0.0f, 0.0f }),
		m_rotation(0.0f),
		m_lifespanOffset(0.0f), m_lifespan(0.0f),
		m_colorOffset(0), m_rotationOffset(0.0f),
		m_scaleOffset({ 0.0f, 0.0f }), m_positionOffset({ 0.0f, 0.0f }), m_velocityOffset({ 0.0f, 0.0f }),
		m_colorStart(MINT_WHITE()), m_colorEnd(MINT_WHITE())
	{
	}

	SParticleDefinition::SParticleDefinition(const SParticleDefinition& rh)
	{
		m_positionStart = rh.m_positionStart;
		m_positionEnd = rh.m_positionEnd;
		m_scaleStart = rh.m_scaleStart;
		m_scaleEnd = rh.m_scaleEnd;
		m_rotation = rh.m_rotation;
		m_colorStart = rh.m_colorStart;
		m_colorEnd = rh.m_colorEnd;
		m_lifespan = rh.m_lifespan;
		m_lifespanOffset = rh.m_lifespanOffset;
		m_velocity = rh.m_velocity;
		m_velocityOffset = rh.m_velocityOffset;
		m_scaleOffset = rh.m_scaleOffset;
		m_positionOffset = rh.m_positionOffset;
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

}