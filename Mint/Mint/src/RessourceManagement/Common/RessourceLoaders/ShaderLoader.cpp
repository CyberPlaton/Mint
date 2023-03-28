#include "ShaderLoader.h"


namespace mint
{


	bool CShaderLoader::load_ressource(const String& ressource_type, CAsset& asset)
	{
		MINT_ASSERT(ressource_type == "Shader", "Invalid asset type provided!");

		String vs_name = "vs_"; vs_name += asset.get_asset_source_path();
		String ps_name = "ps_"; ps_name += asset.get_asset_source_path();

		CPath vs_path;
		CPath ps_path;

#if BX_PLATFORM_WINDOWS or BX_PLATFORM_XBOXONE or BX_PLATFORM_WINRT
		vs_path = CFileystem::construct_from("windows", vs_name);
		ps_path = CFileystem::construct_from("windows", ps_name);

#elif BX_PLATFORM_LINUX
		vs_path = CFileystem::construct_from("linux", vs_name);
		ps_path = CFileystem::construct_from("linux", ps_name);

#elif BX_PLATFORM_OSX
		vs_path = CFileystem::construct_from("osx", vs_name);
		ps_path = CFileystem::construct_from("osx", ps_name);

#elif BX_PLATFORM_PS4 or BX_PLATFORM_PS5
#elif BX_PLATFORM_NX
#endif

		auto vs_shader = mint::fx::loadShader(vs_name);
		auto ps_shader = mint::fx::loadShader(ps_name);
		auto program = bgfx::createProgram(vs_shader, ps_shader, true);

		if(bgfx::isValid(program))
		{
			return CShaderManager::Get().add_shader_program(asset.get_asset_name(), program);
		}

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