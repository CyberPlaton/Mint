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


	void CMaterialManager::add_material_for_entity(entt::entity entity, const SMaterialDefinition& material_definition)
	{
		auto h = SCAST(u64, entity);
		
		auto hh = mint::algorithm::djb_hash(material_definition.m_materialName);
		
		CMaterial* material = nullptr;


		if (m_materials.find(h) == m_materials.end())
		{
			m_materials[h].initialize(MINTFX_MATERIAL_COUNT_MAX);
		}

		if (!m_materials[h].lookup(hh))
		{
			material = m_materials[h].emplace(hh);
		}
		else
		{
			material = m_materials[h].get(hh);
		}

		MINT_ASSERT(material != nullptr, "Invalid operation. Material was not found!");

		material->read_definition(material_definition);
	}


	mint::CMap2< mint::fx::CMaterial >& CMaterialManager::get_materials_for_entity(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if(m_materials.find(h) == m_materials.end())
		{
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


	void CMaterialManager::set_default_main_material_for_entity(entt::entity entity, const String& default_texture /*= "DefaultSprite"*/, const String& default_shader /*= "Sprite"*/, BlendMode blending_mode/*= BLEND_ALPHA*/, BlendingEquation blending_equation /*= BlendingEquation_BlendColor*/, BlendingFactor blending_src_factor /*= BlendingFactor_SrcAlpha*/, BlendingFactor blending_dst_factor /*= BlendingFactor_OneMinusSrcAlpha*/)
	{
		auto h = SCAST(u64, entity);
		
		SMaterialDefinition def;

		def.m_materialName = "Default Material";
		def.m_textureName = default_texture;
		def.m_shaderProgramName = default_shader;
		def.m_blendMode = blending_mode;
		def.m_srcBlendFactor = blending_src_factor;
		def.m_dstBlendFactor = blending_dst_factor;
		def.m_blendingEquation = blending_equation;

		auto hh = mint::algorithm::djb_hash(def.m_materialName);

		auto& map = m_materials[h];

		if (!map.lookup(hh))
		{
			auto material = map.emplace(hh);

			material->read_definition(def);
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


}