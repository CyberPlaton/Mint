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
		}
	}


	bool CApplicationComponentsDatabase::add_component_to_entity(const String& component_name, entt::entity entity)
	{
		auto h = mint::algorithm::djb_hash(component_name);

		if(!m_componentAddFunctions.lookup(h))
		{
			return false;
		}

		auto& registry = MINT_ACTIVE_SCENE()->get_registry()->get_entt_registry_ref();

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
		MINT_LOG_INFO("Dumping registered components:");

		for(const auto& c : m_componentNames)
		{
			MINT_LOG_INFO("\t\"{}\"", c.c_str());
		}

		MINT_LOG_WARN("\tNote: Components are registered only as they are created as an object at least once during the lifetime of the program.");
		MINT_LOG_SEPARATOR();
	}


	bool CApplicationComponentsDatabase::remove_component_from_entity(const String& component_name, entt::entity entity)
	{
		auto h = mint::algorithm::djb_hash(component_name);

		if (!m_componentRemoveFunctions.lookup(h))
		{
			return false;
		}

		auto& registry = MINT_ACTIVE_SCENE()->get_registry()->get_entt_registry_ref();

		m_componentRemoveFunctions.get(h)(registry, entity);



		return true;
	}


}

#endif