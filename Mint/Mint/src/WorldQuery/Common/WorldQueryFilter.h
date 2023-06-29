#ifndef _MINT_WORLD_QUERY_FILTER_H_
#define _MINT_WORLD_QUERY_FILTER_H_


#include "WorldQueryProxy.h"


namespace mint::world
{
	class CEdge;
	class CNode;

	class CWorldQueryFilter
	{
	public:
		virtual bool does_proxy_pass_filter(const SWorldQueryProxy&) = 0;
	};


	class CWorldQueryDatabaseFilter
	{
	public:
		virtual bool does_edge_pass_filter(CEdge*) { return true; };
		virtual bool does_from_node_pass_filter(CNode*) { return true; };
		virtual bool does_to_node_pass_filter(CNode*) { return true; };
	};
}


#endif