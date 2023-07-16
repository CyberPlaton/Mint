#ifndef _MINT_UI_COMMON_H_
#define _MINT_UI_COMMON_H_


#include "Common/Common.h"
#include "Common/Rectangle.h"
#include "Graphics/Common/Color.h"

#include "imgui/rlImGui.h"
#include "imgui/imgui.h"
#include "imgui/imgui-knobs.h"
#include "imgui/implot.h"

namespace ImGui
{
	bool BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);
	bool Spinner(const char* label, float radius, int thickness, const ImU32& color);
}


#endif
