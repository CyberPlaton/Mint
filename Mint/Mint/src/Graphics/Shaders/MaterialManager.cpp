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

			m_materials.reset();

			);

	}


	void CMaterialManager::add_material_for_entity(entt::entity entity, const SMaterialDefinition& material_definition)
	{
		auto h = SCAST(u64, entity);

		if (!m_materials.lookup(h))
		{
			m_materials.add(h, Vector< CMaterial >{});
		}
		
		auto& vector = m_materials.get_ref(h);

		vector.emplace_back(material_definition);
	}


	const mint::Vector< mint::fx::CMaterial >& CMaterialManager::get_materials_for_entity(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if(!m_materials.lookup(h))
		{
			auto& materials = m_materials.emplace_back(h);

			set_default_main_material_for_entity(entity);
		}

		return m_materials.get_const(h);
	}


	const mint::fx::CMaterial& CMaterialManager::get_main_material_for_entity(entt::entity entity)
	{
		const auto& materials = get_materials_for_entity(entity);

		return materials[0];
	}


	void CMaterialManager::set_default_main_material_for_entity(entt::entity entity, const String& default_texture /*= "DefaultSprite"*/, const String& default_shader /*= "Sprite"*/, BlendMode blending_mode/*= BLEND_ALPHA*/, BlendingEquation blending_equation /*= BlendingEquation_BlendColor*/, BlendingFactor blending_src_factor /*= BlendingFactor_SrcAlpha*/, BlendingFactor blending_dst_factor /*= BlendingFactor_OneMinusSrcAlpha*/)
	{
		auto h = SCAST(u64, entity);

		auto& materials = m_materials.get_ref(h);
		auto& material = materials.emplace_back();

		material.set_shader_program(default_shader);
		material.set_texture(default_texture);
		material.set_blend_mode(blending_mode);
		material.set_blend_mode_equation(blending_equation);
		material.set_blend_mode_src_factor(blending_src_factor);
		material.set_blend_mode_dst_factor(blending_dst_factor);
	}


}