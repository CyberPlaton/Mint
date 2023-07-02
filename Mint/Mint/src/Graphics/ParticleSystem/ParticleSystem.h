#ifndef _MINTFX_PARTICLE_SYSTEM_H_
#define _MINTFX_PARTICLE_SYSTEM_H_


#include "Common/Common.h"
#include "../Common/IRenderingPass.h"
#include "Utility/PluginSystem/Plugin.h"
#include "RessourceManagement/TextureManager.h"

#include "Utility/STL/BinarySearchTree.h"
#include "Utility/Profiling/FunctionProfiler.h"
#include "Utility/Logging/Logging.h"

#include "Common/ParticleEmitter.h"


namespace mint::fx
{

	class CParticleSystem : public CRenderingPass
	{
	public:
		STATIC_GET(CParticleSystem, s_CParticleSystem);


		bool initialize() override final;

		void terminate() override final;

		bool requires_termination() override final;

		void on_update(f32 dt);
		
		void on_frame(Vector< entt::entity >& entities) override final;


		bool request_entity_registration(entt::entity entity);

		bool is_entity_registered(entt::entity entity);

		void create_particle_emitter_prefab(const String& particle_emitter_name, const String& particle_emitter_file_path);

		bool set_entity_particle_emitter(entt::entity entity, const String& particle_emitter_name);

		void remove_entity_particle_emitter(entt::entity entity);

		CParticleEmitter* get_particle_emitter_for_entity(entt::entity entity);

	private:
		CBinarySearchTree< CParticleEmitter > m_emitters;

		CMap< std::pair< String, String > > m_particleEmitterPrefabs;


	};

}

#endif