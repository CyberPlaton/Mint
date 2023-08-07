#ifndef _MINT_RANDOM_H_
#define _MINT_RANDOM_H_

#include "Common.h"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#include <random>

namespace mint
{

	class CRandom
	{
	public:
		CRandom();
		~CRandom() = default;

		f32 alternating_one_float();

		s32 alternating_one_int();

		f32 normalized_float();

		f32 in_range_float(f32 min, f32 max);

		u64 integer();

		u64 in_range_integer(u64 min, u64 max);

		Vec2 vector2();

		Vec2 in_range_vector2(f32 min, f32 max);

	private:
		std::mt19937_64 s_randomEngine;
		std::uniform_int_distribution<std::mt19937_64::result_type> s_distribution;
	};

}

#endif