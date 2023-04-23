#ifndef _MINTFX_COLOR_H_
#define _MINTFX_COLOR_H_


#include "Common/Common.h"


namespace mint::fx
{
	class CColor
	{
	public:
		CColor() = default;
		CColor(u8 r, u8 g, u8 b, u8 a);
		CColor(u32 rgba, bool is_argb = true);
		CColor(mint::Vec4 vec, bool is_normalized = false);
		
		void set_color(u8 r, u8 g, u8 b, u8 a);

		mint::Vec4 to_normalized_color_vec4();

		u32 as_rgba();

		u32 as_argb();

		u32 as_abgr();
		
		mint::Vec4 as_vec4() const;

		Color as_cliteral();

		Color as_cliteral() const;


		u8 m_r;
		u8 m_g;
		u8 m_b;
		u8 m_a;
	};
}


#endif