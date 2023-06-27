#ifndef _MINT_WORLD_QUERY_DATABASE_RESULT_TYPE_H_
#define _MINT_WORLD_QUERY_DATABASE_RESULT_TYPE_H_


namespace mint::world
{

	enum ResultType
	{
		ResultType_None = 0,
		ResultType_Node,
		ResultType_Relationship,
		ResultType_Membership,
		ResultType_Attitude,
		ResultType_Classification
	};

}


#endif