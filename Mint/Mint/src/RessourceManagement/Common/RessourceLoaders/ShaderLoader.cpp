#include "ShaderLoader.h"


namespace mint
{


	bool CShaderLoader::load_ressource(const String& ressource_type, CAsset& asset)
	{
		MINT_ASSERT(ressource_type == "Shader", "Invalid asset type provided!");

		CFileystem fs(CFileystem::get_working_directory());
		CFileystem vs(CFileystem::get_working_directory());

		String vs_name;
		String fs_name;

		fs.forward(asset.get_asset_path());
		vs.forward(asset.get_asset_path());


		if(asset.read_bool("has_vs") && vs.forward("vs_" + asset.get_asset_source_path() + ".vsh"))
		{

			vs_name = vs.get_current_directory().as_string();

// 			vs_name = "vs_"; vs_name += asset.get_asset_path(); vs_name += "/"; vs_name += asset.get_asset_source_path(); vs_name += ".vsh";
		}

// 		fs_name = "fs_"; fs_name += asset.get_asset_path(); vs_name += "/"; vs_name += asset.get_asset_source_path(); fs_name += ".fsh";

		fs.forward("fs_" + asset.get_asset_source_path() + ".fsh");

		fs_name = fs.get_current_directory().as_string();

		CShaderManager::Get().add_shader_program(asset.get_asset_name(), vs_name, fs_name);

		return true;
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