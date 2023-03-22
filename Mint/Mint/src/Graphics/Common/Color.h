#ifndef _MINTFX_COLOR_H_
#define _MINTFX_COLOR_H_


#include "Common/Common.h"


namespace mint::fx
{
	class CColor
	{
	public:
		CColor(u32 r, u32 g, u32 b, u32 a);
		CColor(u32 rgba, bool is_argb = true);
		CColor(mint::Vec4 vec, bool is_normalized = false);
		
		CColor to_normalized_color();

		u32 as_rgba();

		u32 as_argb();
		
		mint::Vec4 as_vec4();



	private:
		mint::Vec4 m_normalized;

		u32 m_r;
		u32 m_g;
		u32 m_b;
		u32 m_a;
	};
}


#endif