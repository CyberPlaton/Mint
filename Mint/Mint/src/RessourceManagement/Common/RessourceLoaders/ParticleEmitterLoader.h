#ifndef _MINT_PARTICLE_EMITTER_LOADER_H_
#define _MINT_PARTICLE_EMITTER_LOADER_H_


#include "IRessourceLoader.h"

#include "Graphics/ParticleSystem/ParticleSystem.h"


namespace mint
{
	class CParticleEmitterLoader : public IRessourceLoader
	{
	public:
		MINT_DEFINE_RESSOURCE_LOADER_CTOR(CParticleEmitterLoader);

		bool load_ressource(const String& ressource_type, CAsset& asset) override final;

		CAsset load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node) override final;
	};
}


#endif