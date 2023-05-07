#if MINT_DISTR
#else
#include "ApplicationComponentsDatabaseInterface.h"


namespace mint::editor
{
	mint::editor::IApplicationComponentsDatabase* IApplicationComponentsDatabase::s_IApplicationComponentsDatabase = nullptr;


	mint::editor::IApplicationComponentsDatabase* IApplicationComponentsDatabase::get_component_database()
	{
		return s_IApplicationComponentsDatabase;
	}


	void IApplicationComponentsDatabase::set_component_database(IApplicationComponentsDatabase* db)
	{
		s_IApplicationComponentsDatabase = db;
	}


}

#endif