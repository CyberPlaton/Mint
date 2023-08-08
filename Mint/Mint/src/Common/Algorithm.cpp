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


	bool are_values_sufficiently_different(f32 lh, f32 rh, f32 epsilon /*=glm::epsilon< f32 >()*/)
	{
		f32 d = glm::abs(lh - rh);

		return d > epsilon;
	}

	bool are_values_sufficiently_different(Vec2 lh, Vec2 rh, f32 epsilon /*=glm::epsilon< f32 >()*/)
	{
		const f32 diff_x = glm::abs(lh.x - rh.x);
		const f32 diff_y = glm::abs(lh.y - rh.y);

		const bool x = diff_x > epsilon;
		const bool y = diff_y > epsilon;

		return x || y;
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

	bool string_does_substr_exist(const String& string, const String& substring)
	{
		return string.find(substring) != std::string::npos;
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

	bool is_value_in_between(f32 value, f32 lh, f32 rh)
	{
		return (

			value >= lh &&
			value <= rh

			);
	}

	bool is_value_in_between(Vec2 value, Vec2 lh, Vec2 rh)
	{
		return (

			value.x >= lh.x &&
			value.y >= lh.y &&
			value.x <= rh.x &&
			value.y <= rh.y

			);
	}

	u64 string_to_number(const String& string)
	{
		return stoul(string);
	}

	b2AABB compute_aabb(const mint::CRect& destination_rect)
	{
		auto x = destination_rect.get_x();
		auto y = destination_rect.get_y();
		auto w = destination_rect.get_width();
		auto h = destination_rect.get_height();

		return compute_aabb({ x + w / 2.0f, y + h / 2.0f }, { w / 2.0f, h / 2.0f });
 	}

	b2AABB compute_aabb(const Vec2& point, const Vec2& halfextents)
	{
		b2AABB aabb;

		aabb.lowerBound = { point.x - halfextents.x, point.y - halfextents.y };
 		aabb.upperBound = { point.x + halfextents.x, point.y + halfextents.y };

		return aabb;
	}

	mint::CRect compute_rect_from_aabb(const b2AABB& aabb)
	{
		mint::CRect rect;

		b2Vec2 center = aabb.GetCenter();
		b2Vec2 halfheights = aabb.GetExtents();

		rect.set_rectangle(center.x - halfheights.x, center.y - halfheights.y, halfheights.x * 2, halfheights.y * 2);

		return rect;
	}

	mint::Vec2 vec2_compute_center_between_two_vectors(const Vec2& p1, const Vec2& p2)
	{
		return { 0.5f * (p1.x + p2.x), 0.5f * (p1.y + p2.y) };
	}

	mint::Vec2 vec2_compute_perpendicular_vector(const Vec2& vec)
	{
		return { -vec.y, vec.x };
	}

	mint::u32 compute_percent_value(f32 total_value, f32 part_value)
	{
		return (part_value / total_value) * 100;
	}

	mint::f32 compute_value_from_percent(f32 total_value, u32 percent)
	{
		if (percent < 1) percent = 1;
		if (percent > 100) percent = 100;

		return total_value * (SCAST(f32, percent) / 100.0f);
	}

	mint::f32 vec2_compute_distance_between_two_vectors(const Vec2& p1, const Vec2& p2)
	{
		f32 dx = (p1.x - p2.x) * (p1.x - p2.x);
		f32 dy = (p1.y - p2.y) * (p1.y - p2.y);
		
		return glm::sqrt(dx + dy);
	}

	mint::f32 vec3_compute_distance_between_two_vectors(const Vec3& p1, const Vec3& p2)
	{
		f32 dx = (p1.x - p2.x) * (p1.x - p2.x);
		f32 dy = (p1.y - p2.y) * (p1.y - p2.y);
		f32 dz = (p1.z - p2.z) * (p1.z - p2.z);


		return glm::sqrt(dx + dy + dz);
	}

	void vec2_rotate_around_point(Vec2& vec, f32 angle, Vec2 rotation_pivot_point)
	{
		Vec2 temp{};
		f32 s = glm::sin(angle);
		f32 c = glm::cos(angle);

		// Translate to rotate around point.
		vec = vec - rotation_pivot_point;
		
		temp.x = vec.x * c - vec.y * s;
		temp.y = vec.x * s + vec.y * c;
		
		// Translate back to previous position.
		vec = temp + rotation_pivot_point;
	}

}