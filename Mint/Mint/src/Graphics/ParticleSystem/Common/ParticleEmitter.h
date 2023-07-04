#ifndef _MINTFX_PARTICLE_EMITTER_H_
#define _MINTFX_PARTICLE_EMITTER_H_


#include "Particle.h"
#include "Utility/Logging/Logging.h"
#include "Utility/STL/Common/ObjectAllocator.h"
#include "Graphics/Shaders/MaterialManager.h"
#include "Graphics/Shaders/Materials/MaterialDefinition.h"
#include "Common/easing.h"
#include "Common/Random.h"


namespace mint::fx
{
	
	class CParticleEmitter
	{
	public:
		CParticleEmitter();
		~CParticleEmitter();


		bool initialize(u64 particle_count = MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX);

		void terminate();

		bool load_particle_emitter_for_entity_from_file(entt::entity entity, const String& particle_emitter_file_path);


		void on_update(f32 dt);


		bool set_material(entt::entity entity, const String& material_name);

		CMaterial* get_material(entt::entity entity);

		const std::array< SParticle, MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX >& get_particles();


		
		void emit(u32 particles);



		f32 get_particles_per_second();
		f32 get_particles_emission_rate();
		void set_particles_per_second(f32 value);
		void set_particles_emission_rate(f32 value);

		bx::Easing::Enum get_position_ease();
		bx::Easing::Enum get_scale_ease();
		bx::Easing::Enum get_rotation_ease();
		bx::Easing::Enum get_color_ease();

		void set_position_ease(bx::Easing::Enum value);
		void set_scale_ease(bx::Easing::Enum value);
		void set_rotation_ease(bx::Easing::Enum value);
		void set_color_ease(bx::Easing::Enum value);

		SParticleDefinition& get_particle_definition();


	private:
		std::array< SParticle, MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX > m_particles;

		//CObjectAllocator< SParticle > m_particles;
		
		SParticleDefinition m_particleDefinition;

		u32 m_currentParticleIndex;

		f32 m_particlesPerSecond = 0.0f;

		f32 m_particlesEmissionRate = 0.0f;

		bx::Easing::Enum m_positionEase = bx::Easing::Linear;
		bx::Easing::Enum m_scaleEase = bx::Easing::Linear;
		bx::Easing::Enum m_rotationEase = bx::Easing::Linear;
		bx::Easing::Enum m_colorEase = bx::Easing::Linear;

		
	private:
		f32 get_current_easing_t_between_zero_and_one(f32 particle_life, bx::Easing::Enum easing_function);

	};

}

#endif