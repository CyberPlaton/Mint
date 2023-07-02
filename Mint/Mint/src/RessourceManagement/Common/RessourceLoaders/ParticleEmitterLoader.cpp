#include "ParticleEmitterLoader.h"


namespace mint
{


	bool CParticleEmitterLoader::load_ressource(const String& ressource_type, CAsset& asset)
	{
		MINT_ASSERT(ressource_type == "ParticleEmitter", "Invalid asset type provided!");

		String name, type, source;
		u64 w, h;

		name = asset.get_asset_name();
		type = asset.get_ressource_type();
		source = asset.get_asset_source_path();

		CFilesystem fs(CFilesystem::get_working_directory());
		if(fs.forward(asset.get_asset_path()) && fs.forward(source))
		{
			fx::CParticleSystem::Get().create_particle_emitter_prefab(name, fs.get_current_directory().as_string());

			return true;
		}

		return false;
	}


	mint::CAsset CParticleEmitterLoader::load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node)
	{
		MINT_ASSERT(ressource_type == "ParticleEmitter", "Invalid asset type provided!");
		
		CAsset asset;
		String type, name, source;

		CSerializer::import_string(type, "type", node);
		CSerializer::import_string(name, "name", node);
		CSerializer::import_string(source, "source", node);


		asset.set_asset_path(ressource_container_folder);
		asset.set_asset_name(name);
		asset.set_ressource_type(type);
		asset.set_asset_source_path(source);

		return asset;
	}


}