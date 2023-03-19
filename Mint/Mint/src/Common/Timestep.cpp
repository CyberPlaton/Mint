#include "Timestep.h"

namespace mint
{


	mint::f32 CTimestep::get_fps()
	{
		auto stats = bgfx::getStats();

		return stats->cpuTimeFrame;
	}


	mint::f32 CTimestep::get_frametime()
	{
		auto stats = bgfx::getStats();

		return 1000.0f / stats->cpuTimeFrame;
	}


}