#include "Timestep.h"

namespace mint
{


	mint::f32 CTimestep::get_fps()
	{
		return 60.0f;
	}


	mint::f32 CTimestep::get_frametime()
	{
		return 0.016f;
	}


}