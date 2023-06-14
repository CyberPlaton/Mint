#include "MaterialManager.h"


namespace mint::fx
{
	mint::fx::CMaterialManager* CMaterialManager::s_CMaterialManager;


	bool CMaterialManager::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}


	void CMaterialManager::terminate()
	{
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CMaterialManager::reset()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_materials.clear();

			);

	}


	bool CMaterialManager::add_material_for_entity(entt::entity entity, const SMaterialDefinition& material_definition)
	{
		auto h = SCAST(u64, entity);
		
		auto hh = mint::algorithm::djb_hash(material_definition.m_materialName);
		
		CMaterial* material = nullptr;


		if (m_materials.find(h) == m_materials.end())
		{
			// Entity does not have any Materials yet.
			m_materials[h].initialize(MINTFX_MATERIAL_COUNT_MAX);
		}


		if (!m_materials[h].lookup(hh))
		{
			// Create a new material entry.
			material = m_materials[h].emplace(hh);
		}
		else
		{
			// Replace the existing material with new definition.
			material = m_materials[h].get(hh);
		}

		MINT_ASSERT(material != nullptr, "Invalid operation. Material was not found!");

		if (material)
		{
			material->read_definition(material_definition);

			material->bind_static_uniforms();

			return true;
		}

		return false;
	}


	bool CMaterialManager::set_material_for_entity_at_index(entt::entity entity, const SMaterialDefinition& material_definition, u64 index)
	{
		auto h = SCAST(u64, entity);

		// Assure correct index..
		if (index < m_materials[h].size())
		{
			// .. for replacing.
			auto hh = mint::algorithm::djb_hash(material_definition.m_materialName);

			CMaterial* material = nullptr;

			// Make sure that the name is not already taken.
			if (!m_materials[h].lookup(hh))
			{
				// Replace existing material with new one.
				material = m_materials[h].replace(hh, index);
			}
			else
			{
				// Duplicate Material name.
				return false;
			}

			MINT_ASSERT(material != nullptr, "Invalid operation. Material was not found!");

			if (material)
			{
				material->read_definition(material_definition);

				material->bind_static_uniforms();

				return true;
			}

			return false;
		}
		else
		{
			// .. for emplacing.
			return add_material_for_entity(entity, material_definition);
		}
	}

	bool CMaterialManager::set_material_for_entity_at_index(const String& material_name, u64 index, entt::entity entity)
	{
		auto material_handle = mint::algorithm::djb_hash(material_name);

		if (!m_materialPrefabs.lookup(material_handle)) return false;

		String material_file_path = m_materialPrefabs.get(material_handle).second;

		maml::CDocument document;

		auto root = CSerializer::load_maml_document(material_file_path, document);
		if (!root)
		{
			return false;
		}

		auto node = document.find_first_match_in_document("material");
		if (!node)
		{
			return false;
		}

		String mat_name;
		String texture_name;
		String shader_name;


		CSerializer::import_string(mat_name, "name", node);
		CSerializer::import_string(texture_name, "texture", node);
		CSerializer::import_string(shader_name, "shader", node);

		u64 blendmode = 0, blendingsrcfactor = 0, blendingdstfactor = 0, blendingequation = 0;

		CSerializer::import_uint(&blendmode, "blendingmode", node);
		CSerializer::import_uint(&blendingsrcfactor, "blendingsrcfactor", node);
		CSerializer::import_uint(&blendingdstfactor, "blendingdstfactor", node);
		CSerializer::import_uint(&blendingequation, "blendingequation", node);


		// Set data for Material Definition.
		mint::fx::SMaterialDefinition materialdef;
		materialdef.m_materialName = mat_name;
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


		return set_material_for_entity_at_index(entity, materialdef, index);
	}


	mint::CMap2< mint::fx::CMaterial >& CMaterialManager::get_materials_for_entity(entt::entity entity)
	{
 		auto h = SCAST(u64, entity);

		if(m_materials.find(h) == m_materials.end())
		{
			MINT_LOG_WARN("[{:.4f}][CMaterialManager::get_materials_for_entity] Entity \"{}\" does not have a material and a default one will be set!", MINT_APP_TIME, h);

			auto& materials = m_materials[h];

			set_default_main_material_for_entity(entity);
		}

		return m_materials[h];
	}


	mint::fx::CMaterial* CMaterialManager::get_main_material_for_entity(entt::entity entity)
	{
		auto& materials = get_materials_for_entity(entity);

		return materials.begin();
	}


	void CMaterialManager::set_default_main_material_for_entity(entt::entity entity)
	{
		auto h = SCAST(u64, entity);
		
		auto hh = mint::algorithm::djb_hash("mat_default");

		// Create a new Material map for the entity.
		auto& map = m_materials[h];

		if (map.initialize(MINTFX_MATERIAL_COUNT_MAX))
		{
			if (!set_material_for_entity(hh, entity))
			{
				MINT_LOG_WARN("[{:.4f}][CMaterialManager::set_default_main_material_for_entity] Failed to set default material for entity \"{}\"!", MINT_APP_TIME, h);
			}

			return;
		}

		MINT_ASSERT(false, "Invalid operation. Setting a second default material for an entity is not allowed!");
	}

	void CMaterialManager::remove_material_from_entity(entt::entity entity, const String& material_name)
	{
		auto h = SCAST(u64, entity);

		auto hh = mint::algorithm::djb_hash(material_name);

		if (m_materials.find(h) != m_materials.end())
		{
			m_materials[h].remove(hh);
		}
	}

	void CMaterialManager::create_material_prefab(const String& material_name, const String& material_file_path)
	{
		auto h = mint::algorithm::djb_hash(material_name);

		m_materialPrefabs.add(h, std::make_pair(material_name.c_str(), material_file_path.c_str()));
	}

	bool CMaterialManager::set_material_for_entity(const String& material_name, entt::entity entity)
	{
		auto h = mint::algorithm::djb_hash(material_name);

		return set_material_for_entity(h, entity);
	}

	bool CMaterialManager::set_material_for_entity(MaterialHandle material_handle, entt::entity entity)
	{
		if (!m_materialPrefabs.lookup(material_handle)) return false;

		String material_file_path = m_materialPrefabs.get(material_handle).second;

		maml::CDocument document;

		auto root = CSerializer::load_maml_document(material_file_path, document);
		if (!root)
		{
			return false;
		}

		auto node = document.find_first_match_in_document("material");
		if (!node)
		{
			return false;
		}

		String mat_name;
		String texture_name;
		String shader_name;


		CSerializer::import_string(mat_name, "name", node);
		CSerializer::import_string(texture_name, "texture", node);
		CSerializer::import_string(shader_name, "shader", node);

		u64 blendmode = 0, blendingsrcfactor = 0, blendingdstfactor = 0, blendingequation = 0;

		CSerializer::import_uint(&blendmode, "blendingmode", node);
		CSerializer::import_uint(&blendingsrcfactor, "blendingsrcfactor", node);
		CSerializer::import_uint(&blendingdstfactor, "blendingdstfactor", node);
		CSerializer::import_uint(&blendingequation, "blendingequation", node);


		// Set data for Material Definition.
		mint::fx::SMaterialDefinition materialdef;
		materialdef.m_materialName = mat_name;
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


		return add_material_for_entity(entity, materialdef);
	}

	void CMaterialManager::_load_uniforms(CMap< mint::fx::SShaderUniform >& uniforms, Vector< maml::SProperty >& properties)
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