#ifndef _MINT_IRESSOURCE_LOADER_H_
#define _MINT_IRESSOURCE_LOADER_H_


#include "Common/Common.h"

#include "RessourceManagement/Common/Asset.h"

#include "Utility/Serialization/Serializer.h"


namespace mint
{
	class IRessourceLoader
	{
	public:
		virtual bool load_ressource(const String&, CAsset&) = 0;

		virtual CAsset load_asset(const String&, const String&, maml::SNode*) = 0;
	};
}


#define MINT_DEFINE_RESSOURCE_LOADER_CTOR(class_name) \
static mint::IRessourceLoader* create() { return new class_name(); }


#endif