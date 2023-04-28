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

		if(fps > 0.0f)
		{
			m_frametime = 1.0f / m_fps;
		}
		else
		{
			m_frametime = 0.0f;
		}
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