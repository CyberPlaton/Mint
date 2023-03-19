#include "IMint.h"

namespace mint
{
	mint::IMintEngine* IMintEngine::s_engine = nullptr;


	mint::IMintEngine* IMintEngine::get_engine()
	{
		return s_engine;
	}

}