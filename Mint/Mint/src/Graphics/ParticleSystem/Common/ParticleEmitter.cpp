#include "ParticleEmitter.h"


namespace mint::fx
{

	CParticleEmitter::CParticleEmitter() : 
		m_currentParticleIndex(MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX - 1)
	{
	}

	CParticleEmitter::~CParticleEmitter()
	{

	}

	bool CParticleEmitter::initialize()
	{
		return true;
	}

	void CParticleEmitter::terminate()
	{
	}

	bool CParticleEmitter::load_particle_emitter_for_entity_from_file(entt::entity entity, const String& particle_emitter_file_path)
	{
		maml::CDocument document;

		auto root = CSerializer::load_maml_document(particle_emitter_file_path, document);
		if (!root)
		{
			MINT_LOG_ERROR("[{:.4f}][CParticleEmitter::load_particle_emitter_for_entity_from_file] Failed importing emitter at \"{}\", as the file is badly defined!", MINT_APP_TIME, particle_emitter_file_path);
			return false;
		}

		auto node = document.find_first_match_in_document("emitter");
		if (!node)
		{
			MINT_LOG_ERROR("[{:.4f}][CParticleEmitter::load_particle_emitter_for_entity_from_file] Failed importing emitter at \"{}\", as the file requires an \"emitter\" node with the definition!", MINT_APP_TIME, particle_emitter_file_path);
			return false;
		}


		// Material.
		String material;
		CSerializer::import_string(material, "material", node);

		fx::SMaterialDefinition material_definition;

		if (!mint::fx::CMaterialManager::Get().set_material_for_entity(material, entity))
		{
			MINT_LOG_ERROR("[{:.4f}][CParticleEmitter::load_particle_emitter_for_entity_from_file] Failed importing emitter at \"{}\", as the material \"{}\" could not be located!", MINT_APP_TIME, particle_emitter_file_path, material);
			return false;
		}
		
		u64 mode = 0;
		CSerializer::import_uint(&mode, "mode", node, 0);

		// Import gravity value if gravity turned on, otherwise ignore.
		m_mode = (ParticleEmitterMode)mode;
		if (m_mode == ParticleEmitterMode_Gravity)
		{
			CSerializer::import_vec2(m_particleGravity, "gravity", node);
		}

		// Emission rate.
		CSerializer::import_float(&m_particlesEmissionRate, "emission_rate", node);

		// Tangential velocity.
		CSerializer::import_float(&m_particleDefinition.m_tangentialVelocity, "tangential_velocity", node);
		CSerializer::import_vec2(m_particleDefinition.m_tangentialVelocityFalloff, "tangential_velocity_falloff", node);
		CSerializer::import_vec2(m_particleDefinition.m_tangentialVelocityOffset, "tangential_velocity_offset", node);

		// Angular velocity.
		CSerializer::import_float(&m_particleDefinition.m_angularVelocity, "angular_velocity", node);
		CSerializer::import_vec2(m_particleDefinition.m_angularVelocityFalloff, "angular_velocity_falloff", node);
		CSerializer::import_vec2(m_particleDefinition.m_angularVelocityOffset, "angular_velocity_offset", node);

		// Starting direction.
		CSerializer::import_vec2(m_particleDefinition.m_startingDirection, "starting_direction", node);
		CSerializer::import_vec2(m_particleDefinition.m_startingDirectionOffset, "starting_direction_offset", node);

		// Position.
		CSerializer::import_vec2(m_particleDefinition.m_positionStart, "position_start", node);
		CSerializer::import_vec2(m_particleDefinition.m_positionStartOffset, "position_start_offset", node);

		// Scale.
		CSerializer::import_vec2(m_particleDefinition.m_scaleStart, "scale_start", node);
		CSerializer::import_vec2(m_particleDefinition.m_scaleEnd, "scale_end", node);
		CSerializer::import_vec2(m_particleDefinition.m_scaleOffset, "scale_offset", node);

		// Rotation.
		CSerializer::import_float(&m_particleDefinition.m_rotation, "rotation", node);
		CSerializer::import_vec2(m_particleDefinition.m_rotationOffset, "rotation_offset", node);

		// Color.
		Vec4 color;
		CSerializer::import_vec4(color, "color_start", node);
		m_particleDefinition.m_colorStart.set_color(color.r, color.g, color.b, color.a);

		CSerializer::import_vec4(color, "color_end", node);
		m_particleDefinition.m_colorEnd.set_color(color.r, color.g, color.b, color.a);

		// Lifespan.
		CSerializer::import_float(&m_particleDefinition.m_lifespan, "lifespan", node);
		CSerializer::import_vec2(m_particleDefinition.m_lifespanOffset, "lifespan_offset", node);

		return true;
	}

	void CParticleEmitter::emit(u32 particles)
	{
		MINT_PROFILE_SCOPE("Engine::Particles", "CParticleEmitter::emit");

		if (particles <= 0) return;

		CRandom rand;

		for (u32 i = 0; i < particles; i++)
		{
			auto& particle = m_particles[m_currentParticleIndex];

			// Skip already active particles.
			if (particle.m_active) continue;


			// Initialize particle and set starting values.
			particle.m_active = true;
			particle.m_life = 0.0f - rand.in_range_float(m_particleDefinition.m_lifespanOffset.x, m_particleDefinition.m_lifespanOffset.y);


			particle.m_position = m_particleDefinition.m_positionStart + rand.in_range_vector2(m_particleDefinition.m_positionStartOffset.x, m_particleDefinition.m_positionStartOffset.y);
			
			particle.m_lookingDirection = m_particleDefinition.m_startingDirection + rand.in_range_vector2(m_particleDefinition.m_startingDirectionOffset.x, m_particleDefinition.m_startingDirectionOffset.y);

			particle.m_scale = m_particleDefinition.m_scaleStart + rand.in_range_vector2(m_particleDefinition.m_scaleOffset.x, m_particleDefinition.m_scaleOffset.y);
			
			particle.m_rotation = m_particleDefinition.m_rotation + rand.in_range_float(m_particleDefinition.m_rotationOffset.x, m_particleDefinition.m_rotationOffset.y);
			
			particle.m_angularVelocity = m_particleDefinition.m_angularVelocity + rand.in_range_float(m_particleDefinition.m_angularVelocityOffset.x, m_particleDefinition.m_angularVelocityOffset.y);
			
			particle.m_tangentialVelocity = m_particleDefinition.m_tangentialVelocity + rand.in_range_float(m_particleDefinition.m_tangentialVelocityOffset.x, m_particleDefinition.m_tangentialVelocityOffset.y);
			
			particle.m_color = m_particleDefinition.m_colorStart;


			m_currentParticleIndex = (m_currentParticleIndex == 0) ? m_particles.size() - 1 : m_currentParticleIndex - 1;
		}
	}

	bool CParticleEmitter::set_material(entt::entity entity, const String& material_name)
	{
		if (fx::CMaterialManager::Get().set_material_for_entity(material_name, entity))
		{
			return true;
		}

		return false;
	}

	CMaterial* CParticleEmitter::get_material(entt::entity entity)
	{
		return fx::CMaterialManager::Get().get_main_material_for_entity(entity);
	}

	void CParticleEmitter::on_update(f32 dt)
	{
		MINT_PROFILE_SCOPE("Engine::Particles", "CParticleEmitter::on_update");

		m_dt += dt;
		f32 ease = 0.0f;
		CColor color = MINT_WHITE();
		Vec2 perpendicular = { 0.0f, 0.0f };
		
		auto particle = m_particles.begin();
		for (auto& particle : m_particles)
		{
			if (particle.m_active && particle.m_life <= 1.0f)
			{
				particle.m_life += dt * 1.0f / m_particleDefinition.m_lifespan;

				// ... rotation...
				ease = get_current_easing_t_between_zero_and_one(particle.m_life, m_rotationEase);

				particle.m_rotation += m_particleDefinition.m_rotation * dt * ease;

				// Interpolate position...
				
				// Update tangential and angular velocity.
				ease = get_current_easing_t_between_zero_and_one(particle.m_life, m_tangentialVelocityEase);
				particle.m_tangentialVelocity -= glm::lerp(m_particleDefinition.m_tangentialVelocityFalloff.x, m_particleDefinition.m_tangentialVelocityFalloff.y, ease) * dt;

				ease = get_current_easing_t_between_zero_and_one(particle.m_life, m_angularVelocityEase);
				particle.m_angularVelocity -= glm::lerp(m_particleDefinition.m_angularVelocityFalloff.x, m_particleDefinition.m_angularVelocityFalloff.y, ease) * dt;
				

				// Get current looking direction.
				Vec2 tangential = glm::normalize(particle.m_lookingDirection) * particle.m_tangentialVelocity;
				Vec2 angular = mint::algorithm::vec2_compute_perpendicular_vector(glm::normalize(particle.m_lookingDirection)) * particle.m_angularVelocity;

				// Compute the change in looking direction based on current one and set it as current and 
				// add gravitational power to the looking direction.
				Vec2 tmp;
				switch (m_mode)
				{
				case ParticleEmitterMode_Gravity:
				{
					tmp = (tangential + angular + m_particleGravity) * dt;
					break;
				}
				default:
				case ParticleEmitterMode_None:
				case ParticleEmitterMode_Free:
				{
					tmp = (tangential + angular) * dt;
					break;
				}
				}

				particle.m_lookingDirection += tmp;

				tmp = { particle.m_lookingDirection.x * dt, particle.m_lookingDirection.y * dt };
				particle.m_position += tmp;

				// ... scale...
				ease = get_current_easing_t_between_zero_and_one(particle.m_life, m_scaleEase);

				particle.m_scale = glm::lerp(m_particleDefinition.m_scaleStart, m_particleDefinition.m_scaleEnd, ease);

				// ... color...
				ease = get_current_easing_t_between_zero_and_one(particle.m_life, m_colorEase);

				if (mint::algorithm::is_value_in_between(particle.m_life, 0.0f, 0.5f))
				{
					f32 t = ((ease * 100.0f) / 0.5f) / 100.0f;

					color = glm::lerp(m_particleDefinition.m_colorStart.as_vec4(), m_particleDefinition.m_colorHalf.as_vec4(), t);
				}
				else if (mint::algorithm::is_value_in_between(particle.m_life, 0.5f, 1.0f))
				{
					f32 t = (((ease - 0.5f) * 100.0f) / 0.5f) / 100.0f;

					color = glm::lerp(m_particleDefinition.m_colorHalf.as_vec4(), m_particleDefinition.m_colorEnd.as_vec4(), t);
 				}

				particle.m_color = color;

				if (particle.m_life > 1.0f)
				{
					// Deactivate the particle.
					particle.reset();
				}
			}
		}

		if (m_dt > 1.0f / m_particlesEmissionRate)
		{
			emit(1);

			m_dt = 0.0f;
		}
	}

	glm::f32 CParticleEmitter::get_current_easing_t_between_zero_and_one(f32 particle_life, bx::Easing::Enum easing_function)
	{
		return bx::getEaseFunc(easing_function)(particle_life / 1.0f);
	}

	const std::array< SParticle, MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX >& CParticleEmitter::get_particles()
	{
		return m_particles;
	}

	bx::Easing::Enum CParticleEmitter::get_scale_ease()
	{
		return m_scaleEase;
	}

	bx::Easing::Enum CParticleEmitter::get_rotation_ease()
	{
		return m_rotationEase;
	}

	bx::Easing::Enum CParticleEmitter::get_color_ease()
	{
		return m_colorEase;
	}

	void CParticleEmitter::set_scale_ease(bx::Easing::Enum value)
	{
		m_scaleEase = value;
	}

	void CParticleEmitter::set_rotation_ease(bx::Easing::Enum value)
	{
		m_rotationEase = value;
	}

	void CParticleEmitter::set_color_ease(bx::Easing::Enum value)
	{
		m_colorEase = value;
	}

	mint::fx::SParticleDefinition& CParticleEmitter::get_particle_definition()
	{
		return m_particleDefinition;
	}

	mint::f32 CParticleEmitter::get_particles_emission_rate()
	{
		return m_particlesEmissionRate;
	}

	void CParticleEmitter::set_particles_emission_rate(f32 value)
	{
		m_particlesEmissionRate = value;
	}

	bx::Easing::Enum CParticleEmitter::get_tangential_velocity_ease()
	{
		return m_tangentialVelocityEase;
	}

	bx::Easing::Enum CParticleEmitter::get_angular_velocity_ease()
	{
		return m_angularVelocityEase;
	}

	void CParticleEmitter::set_tangential_velocity_ease(bx::Easing::Enum value)
	{
		m_tangentialVelocityEase = value;
	}

	void CParticleEmitter::set_angular_velocity_ease(bx::Easing::Enum value)
	{
		m_angularVelocityEase = value;
	}

	mint::Vec2 CParticleEmitter::get_emitter_gravity() const
	{
		return m_particleGravity;
	}

	void CParticleEmitter::set_emitter_gravity(const Vec2& vec)
	{
		m_particleGravity = vec;
	}

	mint::fx::ParticleEmitterMode CParticleEmitter::get_emitter_mode() const
	{
		return m_mode;
	}

	void CParticleEmitter::set_emitter_mode(ParticleEmitterMode mode)
	{
		m_mode = mode;
	}

	mint::Vec2 CParticleEmitter::get_emitter_position() const
	{
		return m_particleDefinition.m_positionStart;
	}

	void CParticleEmitter::set_emitter_position(const Vec2& vec)
	{
		m_particleDefinition.m_positionStart = vec;
	}

}
