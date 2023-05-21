#ifndef _MINT_ALGORITHM_H_
#define _MINT_ALGORITHM_H_


#include "Common.h"

#include <sstream>
#include <algorithm>


namespace mint::algorithm
{
	u64 get_next_power_of_2(u64 value);

	u64 djb_hash(const String& str);

	bool are_values_sufficiently_different(f32 lh, f32 rh);

	void string_split(const String& string, char delimiter, Vector< String >& storage );

	void string_insert(String& string, const String& to_insert_one, u64 index);

	void string_push_front(String& string, const String& to_prepend_one);

	void string_push_back(String& string, const String& to_append_one);

	void string_erase_substr(String& string, const String& substring_to_erase);

	void string_erase_range(String& string, u64 begin, u64 end);

	void string_to_lower(String& string);

	u64 string_get_length(const String& string);

	u64 string_find_substr(const String& string, const String& substring);

	f32 radians_to_degree(f32 radians);
	f32 degree_to_radians(f32 degrees);


	template< typename T >
	void vector_push_back(Vector< T >& destination, Vector< T >& source);

	template< typename T >
	void vector_push_back(Vector< T >& destination, T element);

	template < typename T >
	void vector_erase(Vector< T >& from, T element);

	template < typename T >
	void vector_erase_at(Vector< T >& from, u64 index);

	template < typename T >
	void vector_erase_first(Vector< T >& from);

	template < typename T >
	void vector_erase_last(Vector< T >& from);

	template < typename R, typename T >
	R vector_get_last_element_as(Vector< T >& from);

	template < typename R, typename T >
	R vector_get_first_element_as(Vector< T >& from);

}


template < typename T >
void mint::algorithm::vector_erase_at(Vector< T >& from, u64 index)
{
	from.erase(from.begin() + index);
}


template < typename T >
void mint::algorithm::vector_erase_last(Vector< T >& from)
{
	from.erase(from.begin() + from.size() - 1);
}


template < typename T >
void mint::algorithm::vector_erase_first(Vector< T >& from)
{
	from.erase(from.begin());
}


template < typename R, typename T >
R mint::algorithm::vector_get_first_element_as(Vector< T >& from)
{
	MINT_ASSERT(from.empty() == false, "Invalid operation! Can not access element in an empty Vector.");
	return reinterpret_cast<R>(from[0]);
}


template < typename R, typename T >
R mint::algorithm::vector_get_last_element_as(Vector< T >& from)
{
	return reinterpret_cast<R>(from[from.size() - 1]);
}


template< typename T >
void mint::algorithm::vector_push_back(Vector< T >& destination, T element)
{
	destination.push_back(element);
}


template < typename T >
void mint::algorithm::vector_erase(Vector< T >& from, T element)
{
	u64 i = 0;
	for (const auto& __element : from)
	{
		if (__element == element)
		{
			from.erase(from.begin() + i); return;
		}
		i++;
	}
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