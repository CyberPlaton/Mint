#include "Algorithm.h"


#include <cstring>


namespace mint::algorithm
{


	mint::u64 djb_hash(const String& str)
	{
		u64 length = str.size();
		const char* s = str.c_str();
		u64 hash = length;
		
		for (u64 i = 0; i < length; ++s, ++i)
		{
			hash = ((hash << 5) + hash) + (*s);
		}

		return hash;
	}


	void string_split(const String& string, char delimiter, Vector< String >& storage)
	{
		std::stringstream ss(string.c_str());
		std::string token;

		while(std::getline(ss, token, delimiter))
		{
			storage.emplace_back(token.c_str());
		}
	}


	u32 convert_from_argb(u32 r, u32 g, u32 b, u32 a)
	{
		return (a << 24) | (r << 16) | (g << 8) | b;
	}


	u32 convert_from_argb(Vec4 v)
	{
		return convert_from_argb((u32)v.r, (u32)v.g, (u32)v.b, (u32)v.a);
	}


	f32 radians_to_degree(f32 radians)
	{
		return glm::degrees(radians);
	}


	f32 degree_to_radians(f32 degrees)
	{
		return glm::radians(degrees);
	}


}