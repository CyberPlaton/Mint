#ifndef _MINT_TIMESTEP_H_
#define _MINT_TIMESTEP_H_


#include "Common.h"


namespace mint
{

	class CTimestep
	{
	public:
		static f32 get_real_fps();
		static f32 get_real_frametime();


		void set_fps(f32 fps);
		
		f32 get_fps();

		f32 get_frametime();

	private:
		f32 m_fps;
		f32 m_frametime;
	};

}

#endif