#include "MaterialLoader.h"


namespace mint
{


	bool CMaterialLoader::load_ressource(const String& ressource_type, CAsset& asset)
	{
		String name, type, source;
		u64 w, h;

		name = asset.get_asset_name();
		type = asset.get_ressource_type();
		source = asset.get_asset_source_path();

		CFilesystem fs(CFilesystem::get_working_directory());
		if(fs.forward(asset.get_asset_path()) && fs.forward(source))
		{
			maml::CDocument document;

			auto root = CSerializer::load_maml_document(fs.get_current_directory().as_string(), document);
			if (!root)
			{
				return false;
			}

			auto node = document.find_first_match_in_document("material");
			if (!node)
			{
				return false;
			}

			String material_name;
			String texture_name;
			String shader_name;

			
			CSerializer::import_string(material_name, "name", node);
			CSerializer::import_string(texture_name, "texture", node);
			CSerializer::import_string(shader_name, "shader", node);

			u64 blendmode = 0, blendingsrcfactor = 0, blendingdstfactor = 0, blendingequation = 0;

			CSerializer::import_uint(&blendmode, "blendingmode", node);
			CSerializer::import_uint(&blendingsrcfactor, "blendingsrcfactor", node);
			CSerializer::import_uint(&blendingdstfactor, "blendingdstfactor", node);
			CSerializer::import_uint(&blendingequation, "blendingequation", node);


			// Set data for Material Definition.
			mint::fx::SMaterialDefinition materialdef;
			materialdef.m_materialName = material_name;
			materialdef.m_textureName = texture_name;
			materialdef.m_shaderProgramName = shader_name;
			materialdef.m_blendMode = (BlendMode)blendmode;
			materialdef.m_srcBlendFactor = (u32)blendingsrcfactor;
			materialdef.m_dstBlendFactor = (u32)blendingdstfactor;
			materialdef.m_blendingEquation = (u32)blendingequation;


			// Uniforms. We discern between static and dynamic.
			auto staticuniforms = maml::CDocument::find_first_match_in_node(node, "staticuniforms");
			auto dynamicuniforms = maml::CDocument::find_first_match_in_node(node, "dynamicuniforms");


			if (staticuniforms)
			{
				auto& properties = maml::CDocument::get_all_node_properties(staticuniforms);

				_load_uniforms(materialdef.m_staticUniforms, properties);
			}
			if (dynamicuniforms)
			{
				auto& properties = maml::CDocument::get_all_node_properties(dynamicuniforms);

				_load_uniforms(materialdef.m_dynamicUniforms, properties);
			}

			return true;
		}

		return false;
	}


	mint::CAsset CMaterialLoader::load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node)
	{
		MINT_ASSERT(ressource_type == "Material", "Invalid asset type provided!");
		
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


	void CMaterialLoader::_load_uniforms(CMap< mint::fx::SShaderUniform >& uniforms, Vector< maml::SProperty >& properties)
	{
		for (auto& p : properties)
		{
			auto h = mint::algorithm::djb_hash(p.get_property_name());

			mint::fx::SShaderUniform uniform;


			if (p.is< f32 >())
			{
				auto value = p.cast< f32 >();

				uniform.set(p.get_property_name(), (void*)&value, SHADER_UNIFORM_FLOAT);
			}
			else if (p.is< s64 >())
			{
				auto value = p.cast< s64 >();

				uniform.set(p.get_property_name(), (void*)&value, SHADER_UNIFORM_INT);
			}
			else if (p.is< Vec2 >())
			{
				auto value = p.cast< Vec2 >();

				uniform.set(p.get_property_name(), (void*)&value, SHADER_UNIFORM_VEC2);
			}
			else if (p.is< Vec3 >())
			{
				auto value = p.cast< Vec3 >();

				uniform.set(p.get_property_name(), (void*)&value, SHADER_UNIFORM_VEC3);
			}
			else if (p.is< Vec4 >())
			{
				auto value = p.cast< Vec4 >();

				uniform.set(p.get_property_name(), (void*)&value, SHADER_UNIFORM_VEC4);
			}
			else if (p.is< String >())
			{
				auto value = p.cast< String >();

				// Sampler. Retrieve the Texture identifier and set for uniform.
				const Texture& tex = CTextureManager::Get().get_texture(mint::algorithm::djb_hash(value));

				auto id = tex.GetId();

				uniform.set(p.get_property_name(), (void*)&id, SHADER_UNIFORM_SAMPLER2D);
			}

			uniforms.add(h, uniform);
		}
	}

}