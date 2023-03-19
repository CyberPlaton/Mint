#ifndef _MINTFX_VIEWPORT_H_
#define _MINTFX_VIEWPORT_H_


#include "Common/Common.h"


namespace mint::fx
{
	struct SViewport
	{
		f32 m_top;
		f32 m_left;
		f32 m_bottom;
		f32 m_right;

		bgfx::ViewId m_viewIdentifier;
	};
}


#endif