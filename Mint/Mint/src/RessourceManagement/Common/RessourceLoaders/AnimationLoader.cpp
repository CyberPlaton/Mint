#include "AnimationLoader.h"


namespace mint
{


	bool CAnimationLoader::load_ressource(const String& ressource_type, CAsset& asset)
	{
		MINT_ASSERT(ressource_type == "Animation", "Invalid asset type provided!");

		CFilesystem fs(CFilesystem::get_working_directory());

		if(fs.forward(asset.get_asset_path()) && fs.forward(asset.get_asset_source_path()))
		{
			animation::CAnimationSystem::Get().create_animation_prefab(asset.get_asset_name(), fs.get_current_directory().as_string());

			return true;
		}

		return false;
	}


	mint::CAsset CAnimationLoader::load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node)
	{
		MINT_ASSERT(ressource_type == "Animation", "Invalid asset type provided!");
		
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