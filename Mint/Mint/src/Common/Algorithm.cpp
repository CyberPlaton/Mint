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


	f32 radians_to_degree(f32 radians)
	{
		return glm::degrees(radians);
	}


	f32 degree_to_radians(f32 degrees)
	{
		return glm::radians(degrees);
	}


	bool are_values_sufficiently_different(f32 lh, f32 rh)
	{
		return glm::distance< f32 >(lh, rh) > glm::epsilon< f32 >();
	}


	void string_push_front(String& string, const String& to_prepend_one)
	{
		string.insert(0, to_prepend_one);
	}


	void string_push_back(String& string, const String& to_append_one)
	{
		string.append(to_append_one);
	}


	void string_erase_substr(String& string, const String& substring_to_erase)
	{
		auto index = string.find(substring_to_erase);
		if(index != std::string::npos)
		{
			string.erase(index, substring_to_erase.size());
		}
	}


	void string_erase_range(String& string, u64 begin, u64 end)
	{
		string.erase(string.begin() + begin, string.begin() + end);
	}


	mint::u64 string_find_substr(const String& string, const String& substring)
	{
		return string.find(substring);
	}


	mint::u64 string_get_length(const String& string)
	{
		return string.size();
	}


	void string_insert(String& string, const String& to_insert_one, u64 index)
	{
		string.insert(index, to_insert_one);
	}


	void string_to_lower(String& string)
	{
		std::transform(string.begin(), string.end(), 
					   string.begin(), [](unsigned char c) {return std::tolower(c); });
	}


	u64 get_next_power_of_2(u64 value)
	{
		value--;
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		value++;

		return value;
	}


}