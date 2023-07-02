#include "ParticleEmitter.h"


namespace mint::fx
{

	CParticleEmitter::CParticleEmitter() : 
		m_currentParticleIndex(MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX - 1), m_particlesPerSecond(1.0f)
	{
	}

	CParticleEmitter::~CParticleEmitter()
	{

	}

	bool CParticleEmitter::initialize(u64 particle_count /*= MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX*/)
	{
		bool result = m_particles.initialize(particle_count);

		if (result)
		{
			// Initialize all the particles.
			for (auto i = 0; i < particle_count; i++) m_particles.create();
		}

		return result;
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

		CSerializer::import_vec2(m_particleDefinition.m_velocity, "vel", node);
		CSerializer::import_vec2(m_particleDefinition.m_velocityOffset, "velOffset", node);
		CSerializer::import_vec2(m_particleDefinition.m_positionStart, "posStart", node);
		CSerializer::import_vec2(m_particleDefinition.m_positionEnd, "posEnd", node);
		CSerializer::import_vec2(m_particleDefinition.m_positionOffset, "posOffset", node);
		CSerializer::import_vec2(m_particleDefinition.m_scaleStart, "scaleStart", node);
		CSerializer::import_vec2(m_particleDefinition.m_scaleEnd, "scaleEnd", node);
		CSerializer::import_vec2(m_particleDefinition.m_scaleOffset, "scaleOffset", node);
		CSerializer::import_float(&m_particleDefinition.m_rotation, "rot", node);
		CSerializer::import_float(&m_particleDefinition.m_rotationOffset, "rotOffset", node);
		CSerializer::import_float(&m_particleDefinition.m_lifespan, "lifespan", node);
		CSerializer::import_float(&m_particleDefinition.m_lifespanOffset, "lifespanOffset", node);
		
		Vec4 color;
		u64 color_offset;
		CSerializer::import_vec4(color, "colorStart", node);
		m_particleDefinition.m_colorStart.set_color(color.r, color.g, color.b, color.a);

		CSerializer::import_vec4(color, "colorEnd", node);
		m_particleDefinition.m_colorEnd.set_color(color.r, color.g, color.b, color.a);

		CSerializer::import_uint(&color_offset, "colorOffset", node);
		m_particleDefinition.m_colorOffset = SCAST(u8, color_offset);

		String material;
		CSerializer::import_string(material, "material", node);


		fx::SMaterialDefinition material_definition;

		if (mint::fx::CMaterialManager::Get().set_material_for_entity(material, entity))
		{
			return true;
		}
		else
		{
			MINT_LOG_ERROR("[{:.4f}][CParticleEmitter::load_particle_emitter_for_entity_from_file] Failed importing emitter at \"{}\", as the material \"{}\" could not be located!", MINT_APP_TIME, particle_emitter_file_path, material);
		}

		return false;
	}

	mint::fx::SParticle* CParticleEmitter::begin()
	{
		return m_particles.begin();
	}

	mint::fx::SParticle* CParticleEmitter::advance(SParticle* particle)
	{
		return m_particles.advance(particle);
	}

	void CParticleEmitter::emit(u32 particles)
	{
		const f32 time_per_particle = 1.0f / m_particlesPerSecond;
		const u32 num_particles = u32(particles / time_per_particle);

		for (u32 i = 0; i < num_particles; i++)
		{
			auto particle = m_particles.get(m_currentParticleIndex);

			// Initialize particle and set starting values.
			particle->m_active = true;
			particle->m_position = m_particleDefinition.m_positionStart;
			particle->m_scale = m_particleDefinition.m_scaleStart;
			particle->m_rotation = m_particleDefinition.m_rotation;
			particle->m_color = m_particleDefinition.m_colorStart.as_cliteral();
			particle->m_life = 0.0f;

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
		f32 ease = 0.0f;
		CColor color = MINT_WHITE();

		auto particle = m_particles.begin();
		while (particle)
		{
			if (particle->m_active && particle->m_life < 1.0f)
			{
				particle->m_life += dt * 1.0f / m_particleDefinition.m_lifespan;

				// Interpolate position...
				ease = get_current_easing_t_between_zero_and_one(particle->m_life, m_positionEase);

				particle->m_position = glm::lerp(m_particleDefinition.m_positionStart, m_particleDefinition.m_positionEnd, ease);

				// ... scale...
				ease = get_current_easing_t_between_zero_and_one(particle->m_life, m_scaleEase);

				particle->m_scale = glm::lerp(m_particleDefinition.m_scaleStart, m_particleDefinition.m_scaleEnd, ease);

				// ... rotation...
				ease = get_current_easing_t_between_zero_and_one(particle->m_life, m_rotationEase);

				particle->m_rotation += m_particleDefinition.m_rotation * dt * ease;

				// ... color...
				ease = get_current_easing_t_between_zero_and_one(particle->m_life, m_colorEase);

				color = glm::lerp(m_particleDefinition.m_colorStart.as_vec4(), m_particleDefinition.m_colorEnd.as_vec4(), ease);
				particle->m_color = color.as_cliteral();


				if (particle->m_life >= 1.0f) particle->m_active = false;
			}

			particle = m_particles.advance(particle);
		}

		if (m_particlesPerSecond > 0.0f && m_timer.seconds_elapsed() > 1)
		{
			emit(m_particlesPerSecond);

			m_timer.start_timer();
		}

		if (!m_timer.has_started()) m_timer.start_timer();
	}

	glm::f32 CParticleEmitter::get_current_easing_t_between_zero_and_one(f32 particle_life, bx::Easing::Enum easing_function)
	{
		return bx::getEaseFunc(easing_function)(particle_life / 1.0f);
	}

	mint::CObjectAllocator< mint::fx::SParticle >& CParticleEmitter::get_particles()
	{
		return m_particles;
	}

	mint::f32 CParticleEmitter::get_particles_per_second()
	{
		return m_particlesPerSecond;
	}

	void CParticleEmitter::set_particles_per_second(f32 value)
	{
		m_particlesPerSecond = value;
	}

	bx::Easing::Enum CParticleEmitter::get_position_ease()
	{
		return m_positionEase;
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

	void CParticleEmitter::set_position_ease(bx::Easing::Enum value)
	{
		m_positionEase = value;
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

}
