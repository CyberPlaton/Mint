#include "Layer.h"

namespace mint::editor
{
	mint::f32 CLayer::get_window_width()
	{
		return MINT_ENGINE()->get_main_window_const().get_w();
	}


	mint::f32 CLayer::get_window_height()
	{
		return MINT_ENGINE()->get_main_window_const().get_h();
	}


	mint::f32 CLayer::percent(mint::f32 value, mint::u32 p)
	{
		if (p < 1) p = 1;
		if (p > 100) p = 100;

		return value * ((mint::f32)p / 100.0f);
	}


}




