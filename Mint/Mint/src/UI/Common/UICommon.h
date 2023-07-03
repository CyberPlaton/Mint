#ifndef _MINT_UI_COMMON_H_
#define _MINT_UI_COMMON_H_


#include "Common/Common.h"
#include "Common/Rectangle.h"
#include "Graphics/Common/Color.h"

#include "imgui/rlImGui.h"
#include "imgui/imgui.h"
#include "imgui/imgui-knobs.h"

namespace ImGui
{
	void ToggleButton(const char* str_id, bool* v);
	void ToggleButtonEx(const char* str_id, bool* v, const mint::fx::CColor& bg_active = MINT_GREEN_LIGHT(), const mint::fx::CColor& bg_inactive = MINT_GREY_LIGHT(), const mint::fx::CColor& fg_knob = MINT_WHITE());
}


#endif
