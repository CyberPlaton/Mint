#include "Random.h"

namespace mint
{
	CRandom::CRandom()
	{
		s_randomEngine.seed(std::random_device()());
	}


	f32 CRandom::alternating_one_float()
	{
		return (s_distribution(s_randomEngine) % 2 == 0) ? -1.0f : 1.0f;
	}


	s32 CRandom::alternating_one_int()
	{
		return (s_distribution(s_randomEngine) % 2 == 0) ? -1 : 1;
	}


	f32 CRandom::normalized_float()
	{
		return (f32)s_distribution(s_randomEngine) / std::numeric_limits<int32_t>::max();
	}


	mint::f32 CRandom::in_range_float(f32 min, f32 max)
	{
		return min + normalized_float() * max;
	}

	u32 CRandom::integer()
	{
		return (u32)s_distribution(s_randomEngine);
	}


	mint::u32 CRandom::in_range_integer(u32 min, u32 max)
	{
		return min + u32(normalized_float() * max);
	}

	mint::Vec2 CRandom::vector2()
	{
		return { normalized_float(), normalized_float() };
	}


	mint::Vec2 CRandom::in_range_vector2(f32 min, f32 max)
	{
		return { in_range_float(min, max), in_range_float(min, max) };
	}

}