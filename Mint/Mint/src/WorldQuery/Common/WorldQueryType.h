#ifndef _MINT_WORLD_QUERY_TYPE_H_
#define _MINT_WORLD_QUERY_TYPE_H_


namespace mint::world
{

	enum WorldQueryType
	{
		WorldQueryType_None = 0,
		WorldQueryType_EntityCount,
		WorldQueryType_EntityArray,
		WorldQueryType_AnyOccurrence,
	};

}


#endif