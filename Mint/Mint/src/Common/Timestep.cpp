#include "Timestep.h"

namespace mint
{

	mint::f32 CTimestep::get_real_fps()
	{
		return GetFPS();
	}


	mint::f32 CTimestep::get_real_frametime()
	{
		return GetFrameTime();
	}



	void CTimestep::set_fps(f32 fps)
	{
		m_fps = fps;
		m_frametime = 1 / m_fps;
	}


	mint::f32 CTimestep::get_fps()
	{
		return m_fps;
	}


	mint::f32 CTimestep::get_frametime()
	{
		return m_frametime;
	}


}