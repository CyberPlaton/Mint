#ifndef _MINT_ALGORITHM_H_
#define _MINT_ALGORITHM_H_


#include "Common.h"

#include <sstream>


namespace mint::algorithm
{
	u64 djb_hash(const String& str);

	void string_split(const String& string, char delimiter, Vector< String >& storage );

	u32 convert_from_argb(u32 r, u32 g, u32 b, u32 a);
	u32 convert_from_argb(Vec4 v);

	template< typename T >
	void vector_push_back(Vector< T >& destination, Vector< T >& source);

}



template< typename T >
void mint::algorithm::vector_push_back(Vector< T >& destination, Vector< T >& source)
{
	for(const auto& e: source)
	{
		destination.push_back(e);
	}
}


#endif