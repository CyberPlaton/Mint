#include "ShaderLoader.h"


namespace mint
{


	bool CShaderLoader::load_ressource(const String& ressource_type, CAsset& asset)
	{
		MINT_ASSERT(ressource_type == "Shader", "Invalid asset type provided!");

		String vs_name = "vs_"; vs_name += asset.get_asset_source_path();
		String ps_name = "ps_"; ps_name += asset.get_asset_source_path();

		return false;
	}


	mint::CAsset CShaderLoader::load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node)
	{
		MINT_ASSERT(ressource_type == "Shader", "Invalid asset type provided!");

		CAsset asset;
		String name, type, source;
		
		CSerializer::import_string(name, "name", node);
		CSerializer::import_string(type, "type", node);
		CSerializer::import_string(source, "source", node);
		

		asset.set_asset_name(name);
		asset.set_ressource_type(type);
		asset.set_asset_path(ressource_container_folder);
		asset.set_asset_source_path(source);

		return asset;
	}


}