#include "TextureLoader.h"


namespace mint
{


	bool CTextureLoader::load_ressource(const String& ressource_type, CAsset& asset)
	{
		MINT_ASSERT(ressource_type == "Texture", "Invalid asset type provided!");

		String name, type, source;
		u64 w, h;

		name = asset.get_asset_name();
		type = asset.get_ressource_type();
		source = asset.get_asset_source_path();

		CFilesystem fs(CFilesystem::get_working_directory());
		if(fs.forward(asset.get_asset_path()) && fs.forward(source))
		{
			try 
			{
				Texture texture(fs.get_current_directory().as_string());

				CTextureManager::Get().add_texture(name, texture);

				return true;
			}
			catch(const raylib::RaylibException& e)
			{
			}
		}

		return false;
	}


	mint::CAsset CTextureLoader::load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node)
	{
		MINT_ASSERT(ressource_type == "Texture", "Invalid asset type provided!");
		
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