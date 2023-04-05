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

		u32 integer();


	private:
		std::mt19937 s_randomEngine;
		std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
	};

}

#endif