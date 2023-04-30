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

		mint::Vec4 to_normalized_color_vec4() const;

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


#define MINT_BLACK() mint::fx::CColor(0, 0, 0, 0)
#define MINT_WHITE() mint::fx::CColor(255, 255, 255, 255)

#define MINT_GREY_LIGHT() mint::fx::CColor(255, 255, 255, 255)
#define MINT_GREY() mint::fx::CColor(255, 255, 255, 255)
#define MINT_GREY_DARK() mint::fx::CColor(255, 255, 255, 255)

#define MINT_BLUE_LIGHT() mint::fx::CColor(102, 178, 255, 255)
#define MINT_BLUE() mint::fx::CColor(0, 128, 255, 255)
#define MINT_BLUE_DARK() mint::fx::CColor(0, 0, 153, 255)

#define MINT_RED_LIGHT() mint::fx::CColor(255, 102, 102, 255)
#define MINT_RED() mint::fx::CColor(255, 0, 0, 255)
#define MINT_RED_DARK() mint::fx::CColor(153, 0, 0, 255)

#define MINT_GREEN_LIGHT() mint::fx::CColor(102, 255, 102, 255)
#define MINT_GREEN() mint::fx::CColor(0, 204, 0, 255)
#define MINT_GREEN_DARK() mint::fx::CColor(0, 102, 0, 255)

#define MINT_YELLOW_LIGHT() mint::fx::CColor(255, 255, 102, 255)
#define MINT_YELLOW() mint::fx::CColor(255, 255, 0, 255)
#define MINT_YELLOW_DARK() mint::fx::CColor(153, 153, 0, 255)

#define MINT_PURPLE_LIGHT() mint::fx::CColor(204, 153, 255, 255)
#define MINT_PURPLE() mint::fx::CColor(127, 0, 255, 255)
#define MINT_PURPLE_DARK() mint::fx::CColor(51, 0, 102, 255)


#endif