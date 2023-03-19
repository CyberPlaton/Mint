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
		return SCAST(f32, s_distribution(s_randomEngine) / std::numeric_limits<int32_t>::max());
	}


	u32 CRandom::normalized_int()
	{
		return SCAST(u32, s_distribution(s_randomEngine));
	}


}