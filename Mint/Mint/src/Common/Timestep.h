#ifndef _MINT_TIMESTEP_H_
#define _MINT_TIMESTEP_H_


#include "Common.h"


namespace mint
{

	class CTimestep
	{
	public:
		static f32 get_fps();

		static f32 get_frametime();
	};

}

#endif