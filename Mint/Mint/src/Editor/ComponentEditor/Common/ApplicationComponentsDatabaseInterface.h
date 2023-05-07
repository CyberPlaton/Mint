#if MINT_DISTR
#else
#ifndef _MINTEDITOR_APPLICATION_COMPONENTS_DATABASE_INTERFACE_H_
#define _MINTEDITOR_APPLICATION_COMPONENTS_DATABASE_INTERFACE_H_


#include "Common/Common.h"


namespace mint::editor
{

	class IApplicationComponentsDatabase
	{
	protected:
		typedef void (*ComponentFunctionType)(entt::registry&, entt::entity);

	public:
		static IApplicationComponentsDatabase* get_component_database();
		static void set_component_database(IApplicationComponentsDatabase* db);


		virtual void register_component(const String&, ComponentFunctionType) = 0;

		virtual bool add_component_to_entity(const String&, entt::entity) = 0;
	

	protected:
		static IApplicationComponentsDatabase* s_IApplicationComponentsDatabase;
	};

}


#endif
#endif