#if MINT_DISTR
#else
#ifndef _MINTEDITOR_APPLICATION_COMPONENTS_DATABASE_H_
#define _MINTEDITOR_APPLICATION_COMPONENTS_DATABASE_H_


#include "SceneSystem/Common/IScene.h"
#include "Utility/STL/Map.h"
#include "Common/ApplicationComponentsDatabaseInterface.h"
#include "Physics/PhysicsSystem.h"


namespace mint::editor
{

	class CApplicationComponentsDatabase : public IApplicationComponentsDatabase
	{
	public:
		CApplicationComponentsDatabase();
		~CApplicationComponentsDatabase();


		void register_component(const String& component_name, ComponentFunctionType add_component_to_entity_function,
															  ComponentFunctionType remove_component_from_entity_function) override final;

		bool add_component_to_entity(const String& component_name, entt::entity entity)  override final;

		bool remove_component_from_entity(const String& component_name, entt::entity entity) override final;

		Vector< String >& get_all_component_names() override final;

		void print_registered_components();

	private:
		Vector< String > m_componentNames;

		CMap< ComponentFunctionType > m_componentAddFunctions;

		CMap< ComponentFunctionType > m_componentRemoveFunctions;
	
	};

}


#endif
#endif