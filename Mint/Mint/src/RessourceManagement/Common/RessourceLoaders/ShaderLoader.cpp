#include "ShaderLoader.h"


namespace mint
{


	bool CShaderLoader::load_ressource(const String& ressource_type, CAsset& asset)
	{
		MINT_ASSERT(ressource_type == "Shader", "Invalid asset type provided!");

		String vs_name;
		String fs_name;

		if(asset.read_bool("has_vs"))
		{
			String vs_name = "vs_"; vs_name += asset.get_asset_source_path();
		}

		String ps_name = "fs_"; ps_name += asset.get_asset_source_path();

		Shader shader = LoadShader((asset.read_bool("has_vs") == true) ? vs_name.c_str() : nullptr,
								   fs_name.c_str());

		if(IsShaderReady(shader))
		{
			CShaderManager::Get().add_shader_program(asset.get_asset_name(), shader);

			return true;
		}

		return false;
	}


	mint::CAsset CShaderLoader::load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node)
	{
		MINT_ASSERT(ressource_type == "Shader", "Invalid asset type provided!");

		CAsset asset;
		String name, type, source;
		bool has_vs;
		
		CSerializer::import_string(name, "name", node);
		CSerializer::import_string(type, "type", node);
		CSerializer::import_string(source, "source", node);
		CSerializer::import_bool(&has_vs, "has_vs", node);

		asset.set_asset_name(name);
		asset.set_ressource_type(type);
		asset.set_asset_path(ressource_container_folder);
		asset.set_asset_source_path(source);
		asset.write_bool("has_vs", has_vs);

		return asset;
	}


}