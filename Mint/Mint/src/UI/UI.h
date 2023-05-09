#ifndef _MINT_UI_H_
#define _MINT_UI_H_

#include "Common/UICommon.h"
#include "Utility/FileSystem/Filesystem.h"
#include "Utility/Logging/Logging.h"

namespace mint
{

	class CUI
	{
	public:
		STATIC_GET(CUI, s_CUI);

		bool initialize();

		void terminate();

		void begin();

		void end();

		static bool image_button(const Texture* texture, const Vec2& size, const fx::CColor& bg = MINT_BLACK(), const fx::CColor& tint = MINT_WHITE(), s32 padding = -1);

		static bool edit_field_sint32(s32& value, s32 min, s32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None);

		static bool edit_field_uint32(u32& value, u32 min, u32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None);

		static bool edit_field_uint64(u64& value, u64 min, u64 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None);

		static bool edit_field_f32(f32& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None, f32 speed = 1.0f);

		static bool edit_field_bool(bool& value, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None);

		static bool edit_field_vec2(Vec2& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None, f32 speed = 1.0f);

		static bool edit_field_vec3(Vec3& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None, f32 speed = 1.0f);

		static bool edit_field_vec4(Vec4& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None, f32 speed = 1.0f);

		static bool edit_field_string(String& value, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None);

		static bool edit_field_color(fx::CColor& value, u8 min, u8 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None);

		static bool edit_field_rect(CRect& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None, f32 speed = 1.0f);

		static void help_marker(const String& desc);

		static void help_marker_no_question_mark(const String& desc);

		static bool ui_has_focus();


	private:
		static CUI* s_CUI;

		static f32 s_editDragFieldWidth;
		static f32 s_editScalarFieldWidth;


	private:
		static bool InputTextEx(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
	};


	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback  ChainCallback;
		void* ChainCallbackUserData;
	};

	static s32 InputTextCallback(ImGuiInputTextCallbackData* data);

}

#endif