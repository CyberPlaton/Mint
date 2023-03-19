#include "RessourceLoaderFactory.h"


namespace mint
{
	mint::CMap< mint::CRessourceLoaderFactory::FactoryType > CRessourceLoaderFactory::m_ressourceLoaders;


	mint::IRessourceLoader* CRessourceLoaderFactory::create_ressource_loader(const String& ressource_type)
	{
		auto h = mint::algorithm::djb_hash(ressource_type);

		return m_ressourceLoaders.get(h)();
	}


	void CRessourceLoaderFactory::register_ressource_loader(const String& ressource_type, FactoryType ressource_loader_class_constructor)
	{
		auto h = mint::algorithm::djb_hash(ressource_type);

		m_ressourceLoaders.add(h, ressource_loader_class_constructor);
	}

}