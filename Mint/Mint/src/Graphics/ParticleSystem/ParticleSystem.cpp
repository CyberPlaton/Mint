#include "ParticleSystem.h"

namespace mint::fx
{

	bool CParticleSystem::initialize()
	{
		set_rendering_pass_name("CParticleSystem");

		m_emitters.initialize(MINTFX_PARTICLE_EMITTER_COUNT_MAX);

		return true;
	}

	void CParticleSystem::terminate()
	{
		m_emitters.reset();
		m_particleEmitterPrefabs.reset();
	}

	void CParticleSystem::on_update(f32 dt)
	{
		auto emitter = m_emitters.begin();
		while (emitter)
		{
			emitter->on_update(dt);

			emitter = m_emitters.advance(emitter);
		}
	}

	void CParticleSystem::on_frame(Vector< entt::entity >& entities)
	{
		auto& texture_manager = CTextureManager::Get();

		Vec2 position = { 0.0f, 0.0f };
		Vec2 scale = { 0.0f, 0.0f };
		f32 rotation = 0.0f;
		f32 px = 0.0f;
		f32 py = 0.0f;
		Rectangle dst = { 0.0f, 0.0f, 0.0f, 0.0f };
		Vec2 texture_size = { 0.0f, 0.0f };
		Rectangle src = { 0.0f, 0.0f, 0.0f, 0.0f };


		for (auto& entity : entities)
		{
			if (CUCA::entity_has_component< mint::component::SParticleEmitter >(entity))
			{
				auto emitter = m_emitters.get(SCAST(u64, entity));

				auto material = emitter->get_material(entity);

				// Bind material for all particles to be rendered.
				material->bind_dynamic_uniforms();

				material->bind_shader();

				material->bind_blend_mode();

				texture_size = material->get_texture_dimension();


				src = { 0.0f, 0.0f, texture_size.x,	texture_size.y };


				// Iterate over particles to be rendered in reverse order
				// to avoid rendering artifacts.
				const auto& particles = emitter->get_particles();
				for (auto rit = particles.rbegin(); rit < particles.rend(); ++rit)
				{
					const auto& particle = *rit;

					if (particle.m_active)
					{
						// Compute entity data and particle data for rendering.
						position = particle.m_position;
						scale = particle.m_scale;
						rotation = particle.m_rotation;

						px = position.x - src.width / 2.0f;
						py = position.y - src.height / 2.0f;

						dst = { px + texture_size.x / 2.0f, py + texture_size.y / 2.0f, src.width * scale.x, src.height * scale.y };


						DrawTexturePro(texture_manager.get_texture(material->get_texture_handle()), src, dst, { texture_size.x / 2.0f * scale.x, texture_size.y / 2.0f * scale.y }, mint::algorithm::radians_to_degree(rotation), particle.m_color);
					}
				}


				material->end_blend_mode();

				material->end_shader();
			}
		}
	}

	bool CParticleSystem::request_entity_registration(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (!m_emitters.lookup(h))
		{
			auto emitter = m_emitters.add_node(h);

			if (emitter->initialize())
			{
				return true;
			}

			m_emitters.remove_node(h);
		}

		return false;
	}

	bool CParticleSystem::is_entity_registered(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		return m_emitters.lookup(h);
	}

	void CParticleSystem::create_particle_emitter_prefab(const String& particle_emitter_name, const String& particle_emitter_file_path)
	{
		auto h = mint::algorithm::djb_hash(particle_emitter_name);

		m_particleEmitterPrefabs.add(h, std::make_pair(particle_emitter_name.c_str(), particle_emitter_file_path.c_str()));
	}

	bool CParticleSystem::set_entity_particle_emitter(entt::entity entity, const String& particle_emitter_name)
	{
		auto h = SCAST(u64, entity);
		auto hh = mint::algorithm::djb_hash(particle_emitter_name);

		bool result = false;

		if (m_emitters.lookup(h) && m_particleEmitterPrefabs.lookup(hh))
		{
			auto emitter = m_emitters.get(h);

			result = emitter->load_particle_emitter_for_entity_from_file(entity, m_particleEmitterPrefabs.get(hh).second);
		}

		return result;
	}

	void CParticleSystem::remove_entity_particle_emitter(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_emitters.lookup(h))
		{
			auto emitter = m_emitters.get(h);

			emitter->terminate();

			m_emitters.remove_node(h);
		}
	}

	bool CParticleSystem::requires_termination()
	{
		return false;
	}

	mint::fx::CParticleEmitter* CParticleSystem::get_particle_emitter_for_entity(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_emitters.lookup(h))
		{
			return m_emitters.get(h);
		}

		return nullptr;
	}

}