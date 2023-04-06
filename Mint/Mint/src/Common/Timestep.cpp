#include "Timestep.h"

namespace mint
{


	mint::f32 CTimestep::get_fps()
	{
		const auto stats = bgfx::getStats();

		return 1000.0f / (f32(stats->cpuTimeFrame) * 1000.0f / stats->cpuTimerFreq);
	}


	mint::f32 CTimestep::get_frametime()
	{
		const auto stats = bgfx::getStats();

		return (f32(stats->cpuTimeFrame) * 1000.0f / stats->cpuTimerFreq);
	}


}