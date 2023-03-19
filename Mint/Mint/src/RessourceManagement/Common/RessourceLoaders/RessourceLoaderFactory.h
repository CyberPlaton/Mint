#ifndef _MINT_RESSOURCE_LOADER_FACTORY_H_
#define _MINT_RESSOURCE_LOADER_FACTORY_H_


#include "IRessourceLoader.h"

#include "Utility/STL/Map.h"


namespace mint
{
	class CRessourceLoaderFactory
	{
		typedef IRessourceLoader* (*FactoryType)();
	public:

		static IRessourceLoader* create_ressource_loader(const String& ressource_type);

		static void register_ressource_loader(const String& ressource_type, FactoryType ressource_loader_class_constructor);


	private:
		static CMap< FactoryType > m_ressourceLoaders;
	};
}



#endif