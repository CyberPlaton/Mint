#ifndef _MINT_WORLD_QUERY_FILTER_H_
#define _MINT_WORLD_QUERY_FILTER_H_


#include "WorldQueryProxy.h"


namespace mint::world
{
	class CWorldQueryFilter
	{
	public:
		virtual bool does_proxy_pass_filter(const SWorldQueryProxy&) = 0;
	};

}


#endif