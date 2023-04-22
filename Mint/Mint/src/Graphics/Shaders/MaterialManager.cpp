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

	}


	void CMaterialManager::add_material_for_entity(entt::entity entity, const CMaterial& material)
	{
		auto h = SCAST(u64, entity);

		if(!m_materials.lookup(h)) m_materials.add(h, Vector< CMaterial >{});

		m_materials.get(h).push_back(material);
	}


	const mint::Vector< mint::fx::CMaterial >& CMaterialManager::get_materials_for_entity(entt::entity entity) const
	{
		auto h = SCAST(u64, entity);

		return m_materials.get_const(h);
	}


}