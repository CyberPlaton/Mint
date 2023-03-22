#ifndef _MINT_ALGORITHM_H_
#define _MINT_ALGORITHM_H_


#include "Common.h"

#include <sstream>


namespace mint::algorithm
{
	u64 djb_hash(const String& str);


	void string_split(const String& string, char delimiter, Vector< String >& storage );


	f32 radians_to_degree(f32 radians);
	f32 degree_to_radians(f32 degrees);


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