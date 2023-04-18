#include "Camera.h"


namespace mint::fx
{


	ICamera::ICamera(const CColor& clear_color)
	{

	}


	ICamera::ICamera(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane, const CColor& clear_color)
	{

	}


	void ICamera::set_render_state(u64 state)
	{
	}


	void ICamera::set_view_clear_state(u64 state)
	{
	}


	void ICamera::set_view_clear_color(u8 r, u8 g, u8 b, u8 a)
	{
	}


}