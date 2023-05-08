#if MINT_DISTR
#else
#include "ApplicationComponentsDatabase.h"


namespace mint::editor
{
	void CApplicationComponentsDatabase::register_component(const String& component_name, ComponentFunctionType add_component_to_entity_function, ComponentFunctionType remove_component_from_entity_function)
	{
		auto h = mint::algorithm::djb_hash(component_name);

		if(!m_componentAddFunctions.lookup(h))
		{
			if(component_name == "SRigidBody" && 
			  !CPhysicsSystem::get_use_physics())
			{
				return;
			}

			m_componentAddFunctions.add(h, add_component_to_entity_function);

			m_componentRemoveFunctions.add(h, remove_component_from_entity_function);

			m_componentNames.push_back(component_name);

			MINT_LOG_INFO("[{:.4f}][CApplicationComponentsDatabase::register_component] Registered Component: \"{}\"", MINT_APP_TIME, component_name);
		}
	}


	bool CApplicationComponentsDatabase::add_component_to_entity(const String& component_name, entt::entity entity)
	{
		auto h = mint::algorithm::djb_hash(component_name);

		if(!m_componentAddFunctions.lookup(h))
		{
			return false;
		}

		auto& registry = MINT_ACTIVE_SCENE()->get_registry().get_entt_registry_ref();

		m_componentAddFunctions.get(h)(registry, entity);



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


	mint::Vector< mint::String >& CApplicationComponentsDatabase::get_all_component_names()
	{
		return m_componentNames;
	}


	void CApplicationComponentsDatabase::print_registered_components()
	{
		MINT_LOG_INFO("[{:.4f}][CApplicationComponentsDatabase::print_registered_components] Dumping registered components:", MINT_APP_TIME);

		for(const auto& c : m_componentNames)
		{
			MINT_LOG_INFO("\tComponent: \"{}\"", c.c_str());
		}

		MINT_LOG_WARN("Note the current limitation of the system: We register only components that were created as an object at least once.");
	}


	bool CApplicationComponentsDatabase::remove_component_from_entity(const String& component_name, entt::entity entity)
	{
		auto h = mint::algorithm::djb_hash(component_name);

		if (!m_componentRemoveFunctions.lookup(h))
		{
			return false;
		}

		auto& registry = MINT_ACTIVE_SCENE()->get_registry().get_entt_registry_ref();

		m_componentRemoveFunctions.get(h)(registry, entity);



		return true;
	}


}

#endif