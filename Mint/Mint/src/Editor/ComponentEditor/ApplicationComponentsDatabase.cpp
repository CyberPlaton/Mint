#if MINT_DISTR
#else
#include "ApplicationComponentsDatabase.h"


namespace mint::editor
{
	void CApplicationComponentsDatabase::register_component(const String& component_name, ComponentFunctionType create_function)
	{
		auto h = mint::algorithm::djb_hash(component_name);

		m_components.add(h, create_function);
	}


	bool CApplicationComponentsDatabase::add_component_to_entity(const String& component_name, entt::entity entity)
	{
		auto h = mint::algorithm::djb_hash(component_name);

		if(!m_components.lookup(h))
		{
			return false;
		}

		auto& registry = MINT_ACTIVE_SCENE()->get_registry().get_entt_registry_ref();

		m_components.get(h)(registry, entity);

		return true;
	}


	CApplicationComponentsDatabase::CApplicationComponentsDatabase()
	{
		IApplicationComponentsDatabase::set_component_database(this);
	}


	CApplicationComponentsDatabase::~CApplicationComponentsDatabase()
	{
		IApplicationComponentsDatabase::set_component_database(nullptr);
	}


}

#endif