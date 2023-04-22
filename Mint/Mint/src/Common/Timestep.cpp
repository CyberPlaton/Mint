#include "Timestep.h"

namespace mint
{


	mint::f32 CTimestep::get_fps()
	{
		return GetFPS();
	}


	mint::f32 CTimestep::get_frametime()
	{
		return GetFrameTime();
	}


}