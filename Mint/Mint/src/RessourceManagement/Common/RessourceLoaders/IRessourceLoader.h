#ifndef _MINT_IRESSOURCE_LOADER_H_
#define _MINT_IRESSOURCE_LOADER_H_


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




#endif