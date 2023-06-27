#ifndef _MINT_WORLD_QUERY_DATABASE_QUERY_MODE_H_
#define _MINT_WORLD_QUERY_DATABASE_QUERY_MODE_H_


namespace mint::world
{

	enum QueryMode
	{
		QueryMode_None = 0,
		QueryMode_All,
		QueryMode_Any,
		QueryMode_Count
	};

}


#endif