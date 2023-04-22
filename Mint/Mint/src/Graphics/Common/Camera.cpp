#include "Camera.h"


namespace mint::fx
{


	ICamera::ICamera(const CColor& clear_color) :
		m_clearColor(clear_color),
		m_translation({0.0f, 0.0f}),
		m_translationOffset({ 0.0f, 0.0f }),
		m_zoom(0.0f),
		m_rotation(0.0f)
	{
	}

	void ICamera::set_view_clear_color(u8 r, u8 g, u8 b, u8 a)
	{
		m_clearColor.set_color(r, g, b, a);
	}


}