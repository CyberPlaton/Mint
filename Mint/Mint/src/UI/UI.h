#ifndef _MINT_UI_H_
#define _MINT_UI_H_

#include "Common/UICommon.h"
#include "Utility/FileSystem/Filesystem.h"
#include "Utility/Logging/Logging.h"
#include "Common/Algorithm.h"


namespace mint
{
	enum NotificationType
	{
		NotificationType_None = 0,
		NotificationType_Success,
		NotificationType_Info,
		NotificationType_Warn,
		NotificationType_Error,
		NotificationType_Critical,
	};


	struct SNotification
	{
		static u32 s_SuccessLifetime;
		static u32 s_InfoLifetime;
		static u32 s_WarnLifetime;
		static u32 s_ErrorLifetime;
		static u32 s_CriticalLifetime;

		static ImVec4 s_SuccessColor;
		static ImVec4 s_InfoColor;
		static ImVec4 s_WarnColor;
		static ImVec4 s_ErrorColor;
		static ImVec4 s_CriticalColor;


		NotificationType m_type;
		String m_label;
		String m_message;
		Vec2 m_position;
		Vec2 m_size;
		f32 m_lifetimeFalloff;
		f32 m_lifetime;
	};


	class CUI
	{
	public:
		STATIC_GET(CUI, s_CUI);

		bool initialize();

		void terminate();

		void set_window_dimension(const Vec2& size);

		void begin();

		void end();

		static ImU32 get_imgui_color(f32 r, f32 g, f32 b, f32 a);

		static ImU32 get_imgui_color(const fx::CColor& color);

		static void create_notification(const String& label, const String& message, NotificationType type, const Vec2& size = { 250.0f, 150.0f });

		static void create_notification(const String& label, const String& message, NotificationType type, u32 lifetime_seconds, const Vec2& size);

		static void create_file_dialog(const String& field_text, const String& field_desc, bool* is_open, const Vec2& position, const Vec2& size, const char* allowed_file_types[], u32 file_type_count, const String& directory, const String& ok_text = "OK", const String& cancel_text = "Cancel", ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);

		static void create_folder_dialog(const String& field_text, const String& field_desc, bool* is_open, const Vec2& position, const Vec2& size, const String& directory, const String& ok_text = "OK", const String& cancel_text = "Cancel", ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);

		static void delete_folder_dialog(const String& field_text, const String& field_desc, bool* is_open, const Vec2& position, const Vec2& size, const String& directory, const String& ok_text = "OK", const String& cancel_text = "Cancel");

		static bool is_style_dark();

		static bool is_style_light();

		static bool image_button(const Texture* texture, const Vec2& size, const fx::CColor& bg = MINT_BLACK(), const fx::CColor& tint = MINT_WHITE(), s32 padding = -1);

		static void toggle_button(const String& field_text, bool* v);

		static void toggle_button_ex(const String field_text, bool* v, const mint::fx::CColor& bg_active = MINT_GREEN_LIGHT(), const mint::fx::CColor& bg_inactive = MINT_GREY_LIGHT(), const mint::fx::CColor& fg_knob = MINT_WHITE());

		static void loading_bar(const String& field_id, f32 percent_value, const Vec2& size, const fx::CColor& background = MINT_GREY_DARK(), const fx::CColor& foreground = MINT_GREEN());

		static void loading_circle(const String& field_id, f32 radius, f32 thikness, const fx::CColor& color = MINT_GREEN());

		static bool edit_field_sint32(s32& value, s32 min, s32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None);

		static bool edit_field_sint64(s64& value, s64 min, s64 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None);

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

		static bool edit_field_vec2_ranged(Vec2& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags = ImGuiSliderFlags_None, f32 speed = 1.0f);

		static void help_marker(const String& desc);

		static void help_marker_no_question_mark(const String& desc);

		static bool ui_has_focus();


	private:
		static f32 s_editDragFieldWidth;
		static f32 s_editScalarFieldWidth;

		static Vector< SNotification > s_notifications;
		static Vec2 s_windowSize;


	private:
		/// @brief Reference  
		static bool InputTextEx(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);

		static void show_notifications();
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