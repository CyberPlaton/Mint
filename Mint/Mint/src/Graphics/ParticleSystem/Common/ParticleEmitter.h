#ifndef _MINTFX_PARTICLE_EMITTER_H_
#define _MINTFX_PARTICLE_EMITTER_H_


#include "Particle.h"
#include "Utility/Logging/Logging.h"
#include "Utility/STL/Common/ObjectAllocator2.h"
#include "Graphics/Shaders/MaterialManager.h"
#include "Graphics/Shaders/Materials/MaterialDefinition.h"
#include "Common/Random.h"
#include "Utility/Profiling/FunctionProfiler.h"


namespace mint::fx
{
	
	class CParticleEmitter
	{
	public:
		CParticleEmitter();
		~CParticleEmitter();

		bool initialize();

		void terminate();

		bool load_particle_emitter_for_entity_from_file(entt::entity entity, const String& particle_emitter_file_path);


		void on_update(f32 dt);

		bool is_active() const;
		void set_is_active(bool value);


		bool set_material(entt::entity entity, const String& material_name);

		CMaterial* get_material(entt::entity entity);

		const std::array< SParticle, MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX >& get_particles();


		
		void emit(u32 particles);


		Vec2 get_emitter_position() const;
		void set_emitter_position(const Vec2& vec);

		ParticleEmitterMode get_emitter_mode() const;
		void set_emitter_mode(ParticleEmitterMode mode);


		Vec2 get_emitter_gravity() const;
		void set_emitter_gravity(const Vec2& vec);

		f32 get_particles_emission_rate();
		void set_particles_emission_rate(f32 value);

		bx::Easing::Enum get_tangential_velocity_ease();
		bx::Easing::Enum get_angular_velocity_ease();
		bx::Easing::Enum get_scale_ease();
		bx::Easing::Enum get_rotation_ease();
		bx::Easing::Enum get_color_ease();

		void set_tangential_velocity_ease(bx::Easing::Enum value);
		void set_angular_velocity_ease(bx::Easing::Enum value);
		void set_scale_ease(bx::Easing::Enum value);
		void set_rotation_ease(bx::Easing::Enum value);
		void set_color_ease(bx::Easing::Enum value);

		SParticleDefinition& get_particle_definition();


	private:
		bool m_active = true;

		std::array< SParticle, MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX > m_particles;

		SParticleDefinition m_particleDefinition;

		u32 m_currentParticleIndex;

		f32 m_dt = 0.0f;

		
	private:
		f32 get_current_easing_t_between_zero_and_one(f32 particle_life, bx::Easing::Enum easing_function);

	};

}

#endif