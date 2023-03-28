#include "TextureLoader.h"


namespace mint
{


	bool CTextureLoader::load_ressource(const String& ressource_type, CAsset& asset)
	{
		String name, type, source;
		u64 w, h;

		name = asset.get_asset_name();
		type = asset.get_ressource_type();
		source = asset.get_asset_source_path();

		bgfx::TextureInfo info;

		CFileystem fs(CFileystem::get_working_directory());
		if(fs.forward(asset.get_asset_path()) && fs.forward(source))
		{
			bgfx::TextureHandle handle = fx::loadTexture(fs.get_current_directory().as_string().c_str(),
													 BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, 0, &info);

			if(bgfx::isValid(handle))
			{
				return CTextureManager::Get().add_texture(name, handle, info);
			}
		}
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