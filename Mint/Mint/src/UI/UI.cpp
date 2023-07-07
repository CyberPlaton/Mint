#include "UI.h"
#include "Common/imgui/imgui_internal.h"

namespace mint
{

	CUI* CUI::s_CUI = nullptr;
	f32 CUI::s_editDragFieldWidth = 150.0f;
	f32 CUI::s_editScalarFieldWidth = 100.0f;
	

	bool CUI::InputTextEx(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
	}


	bool CUI::initialize()
	{
		rlImGuiSetup(true);

		bool result = false;

#if MINT_DISTR
		return true;
#else
		CFilesystem fs(CFilesystem::get_working_directory());

		if (fs.forward("EditorRessources") && fs.forward("Fonts"))
		{
			ImGuiIO& io = ImGui::GetIO();

			static const ImWchar ranges[] =
			{
			0x0020, 0x04ff, 0x0FFF,
			0,
			};
			ImFontConfig icons_config; icons_config.MergeMode = false; icons_config.PixelSnapH = true;

			String file = CFilesystem::construct_from(fs.get_current_directory().as_string(), "Cousine-Regular.ttf").as_string();

			auto font = io.Fonts->AddFontFromFileTTF(file.c_str(), 16.0f, &icons_config, ranges);
			
			ImGui::GetIO().FontDefault = font;


			ImFontConfig config;
			config.MergeMode = true;
			config.GlyphMinAdvanceX = 14.5f; // Use if you want to make the icon monospaced
			static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
			
			String icon_file = CFilesystem::construct_from(fs.get_current_directory().as_string(), "fa-solid-900.ttf").as_string();

			io.Fonts->AddFontFromFileTTF(icon_file.c_str(), 14.5f, &config, icon_ranges);

			rlImGuiReloadFonts();

			result = true;
		}
		else
		{
			MINT_LOG_CRITICAL("[{:.4f}][CUI::initialize] Failed loading fonts at \"{}\"!", MINT_APP_TIME, fs.get_current_directory().as_string());
		}

		if (result)
		{
			ImGui::GetStyle().WindowRounding = 0;
			ImGui::GetStyle().ChildRounding = 12;
			ImGui::GetStyle().FrameRounding = 6;
			ImGui::GetStyle().PopupRounding = 12;
			ImGui::GetStyle().ScrollbarRounding = 9;
			ImGui::GetStyle().GrabRounding = 12;
			ImGui::GetStyle().TabRounding = 6;
		}

		return result;
#endif
	}


	void CUI::terminate()
	{
		rlImGuiShutdown();
	}


	void CUI::begin()
	{
		rlImGuiBegin();
	}


	void CUI::end()
	{
		rlImGuiEnd();
	}


	ImU32 CUI::get_imgui_color(f32 r, f32 g, f32 b, f32 a)
	{
		return ImGui::ColorConvertFloat4ToU32({ r, g, b, a });
	}


	ImU32 CUI::get_imgui_color(const fx::CColor& color)
	{
		auto vec = color.to_normalized_color_vec4();
		return ImGui::ColorConvertFloat4ToU32({ vec.r, vec.g, vec.b, vec.a });
	}

	void CUI::create_file_dialog(const String& field_text, const String& field_desc, bool* is_open, const Vec2& position, const Vec2& size,
								 const char* allowed_file_types[], u32 file_type_count, const String& directory,
								 const String& ok_text /*= "OK"*/, const String& cancel_text /*= "Cancel"*/, ImGuiInputTextFlags flags /*= ImGuiInputTextFlags_None*/)
	{
		
		static const char* selected_file_type;
		static s32 selected_file_type_option = -1;
		static char buffer[128];

		ImGui::SetNextWindowPos({ position.x, position.y }, ImGuiCond_Appearing);
		ImGui::SetNextWindowSize({ size.x, size.y }, ImGuiCond_Appearing);

		ImGui::Begin(field_text.c_str(), is_open);


		ImGui::InputText("## File_Naming_Textinput", &buffer[0], sizeof(buffer), flags);


		ImGui::SameLine();

		if (ImGui::SmallButton(selected_file_type_option == -1 ? "Please select a Filetype" : selected_file_type))
		{
			ImGui::OpenPopup("File_Type_Selection_Popup");
		}

		CUI::help_marker_no_question_mark(field_desc);


		if (ImGui::BeginPopup("File_Type_Selection_Popup"))
		{
			for (int i = 0; i < file_type_count; i++)
			{
				if (ImGui::Selectable(allowed_file_types[i])) { selected_file_type_option = i; }
			}

			selected_file_type = allowed_file_types[selected_file_type_option];

			ImGui::EndPopup();
		}



		if (ImGui::SmallButton(ok_text.c_str()))
		{
			// Get the string from text input buffer.
			String file_name = String(buffer);

			// Sanity check for the file and that type was selected.
			if (!file_name.empty() && selected_file_type_option >= 0 && selected_file_type_option < file_type_count)
			{
				// Ready to create file. Get the extension from allowed file types.
				String extension = String(allowed_file_types[selected_file_type_option]);

				// Try creating the file.
				if (CFilesystem::create_file(directory, file_name, extension, false))
				{
					MINT_LOG_INFO("[{:.4f}][CUI::create_file_dialog] Success creating file \"{}\" at \"{}\"!", MINT_APP_TIME, String(file_name + extension), directory);
				}
				else
				{
					MINT_LOG_ERROR("[{:.4f}][CUI::create_file_dialog] Failed creating file \"{}\" at \"{}\"!", MINT_APP_TIME, String(file_name + extension), directory);
				}

				// Reset the internal state.
				*is_open = false;
				selected_file_type_option = -1;
				std::memset(&buffer, NULL, sizeof(buffer));
			}
		}

		ImGui::SameLine();
		
		if (ImGui::SmallButton(cancel_text.c_str()))
		{
			// Reset the internal state.
			*is_open = false;
			selected_file_type_option = -1;
			std::memset(&buffer, NULL, sizeof(buffer));
		}

		ImGui::End();
	}

	void CUI::create_folder_dialog(const String& field_text, const String& field_desc, bool* is_open, const Vec2& position, const Vec2& size, const String& directory, const String& ok_text /*= "OK"*/, const String& cancel_text /*= "Cancel"*/, ImGuiInputTextFlags flags /*= ImGuiInputTextFlags_None*/)
	{
		static char buffer[128];

		ImGui::SetNextWindowPos({ position.x, position.y }, ImGuiCond_Appearing);
		ImGui::SetNextWindowSize({ size.x, size.y }, ImGuiCond_Appearing);

		ImGui::Begin(field_text.c_str(), is_open);


		ImGui::InputText("## Folder_Naming_Textinput", &buffer[0], sizeof(buffer), flags);


		if (ImGui::SmallButton(ok_text.c_str()))
		{
			// Get the string from text input buffer.
			String folder_name = String(buffer);

			if (!folder_name.empty())
			{
				// Try creating folder with specified name at specified location.
				if (CFilesystem::create_directory(CFilesystem::construct_from(directory, folder_name)))
				{
					MINT_LOG_INFO("[{:.4f}][CUI::create_folder_dialog] Success creating folder \"{}\" at \"{}\"!", MINT_APP_TIME, folder_name, directory);
				}
				else
				{
					MINT_LOG_ERROR("[{:.4f}][CUI::create_folder_dialog] Failed creating folder \"{}\" at \"{}\"!", MINT_APP_TIME, folder_name, directory);
				}

				// Reset the internal state.
				*is_open = false;
				std::memset(&buffer, NULL, sizeof(buffer));
			}
		}

		ImGui::SameLine();

		if (ImGui::SmallButton(cancel_text.c_str()))
		{
			// Reset the internal state.
			*is_open = false;
			std::memset(&buffer, NULL, sizeof(buffer));
		}

		ImGui::End();
	}

	void CUI::delete_folder_dialog(const String& field_text, const String& field_desc, bool* is_open, const Vec2& position, const Vec2& size, const String& directory, const String& ok_text /*= "OK"*/, const String& cancel_text /*= "Cancel"*/)
	{
		ImGui::SetNextWindowPos({ position.x, position.y }, ImGuiCond_Appearing);
		ImGui::SetNextWindowSize({ size.x, size.y }, ImGuiCond_Appearing);

		ImGui::Begin(field_text.c_str(), is_open);

		if (ImGui::SmallButton(ok_text.c_str()))
		{
			if (CFilesystem::delete_directory_or_file(directory))
			{
				MINT_LOG_INFO("[{:.4f}][CUI::delete_folder_dialog] Success deleting folder \"{}\"!", MINT_APP_TIME, directory);
			}
			else
			{
				MINT_LOG_ERROR("[{:.4f}][CUI::delete_folder_dialog] Failed deleting folder \"{}\"!", MINT_APP_TIME, directory);
			}

			// Reset the internal state.
			*is_open = false;
		}

		ImGui::SameLine();

		if (ImGui::SmallButton(cancel_text.c_str()))
		{
			// Reset the internal state.
			*is_open = false;
		}

		ImGui::End();
	}

	bool CUI::is_style_light()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		auto color = colors[ImGuiCol_Text];

		return (color.x == 0.00f && color.y == 0.00f && color.z == 0.00f && color.w == 1.00f);
	}


	bool CUI::is_style_dark()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		auto color = colors[ImGuiCol_Text];

		return (color.x == 1.00f && color.y == 1.00f && color.z == 1.00f && color.w == 1.00f);
	}


	bool CUI::image_button(const Texture* texture, const Vec2& size, const fx::CColor& bg /*= MINT_BLACK()*/, const fx::CColor& tint /*= MINT_WHITE()*/, s32 padding /*= 0*/)
	{
		auto bg4 = bg.to_normalized_color_vec4();
		auto tint4 = tint.to_normalized_color_vec4();

		ImGui::PushStyleColor(ImGuiCol_Button, { bg4.r, bg4.g, bg4.b, 0.0f });
		
		bool result = ImGui::ImageButton((ImTextureID)texture, { size.x, size.y }, { 0.0f, 0.0f }, { 1.0f, 1.0f },
								  padding, { bg4.r, bg4.g, bg4.b, bg4.a }, { tint4.r, tint4.g, tint4.b, tint4.a });

		ImGui::PopStyleColor();
		
		return result;
	}


	/// @brief Reference: https://github.com/ocornut/imgui/issues/1537
	void CUI::toggle_button(const String& field_text, bool* v)
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = ImGui::GetFrameHeight();
		float width = height * 1.55f;
		float radius = height * 0.50f;

		ImGui::InvisibleButton(field_text.c_str(), ImVec2(width, height));
		if (ImGui::IsItemClicked())
			*v = !*v;

		float t = *v ? 1.0f : 0.0f;

		ImGuiContext& g = *GImGui;
		float ANIM_SPEED = 0.08f;
		if (g.LastActiveId == g.CurrentWindow->GetID(field_text.c_str()))// && g.LastActiveIdTimer < ANIM_SPEED)
		{
			float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
			t = *v ? (t_anim) : (1.0f - t_anim);
		}

		ImU32 col_bg;
		if (ImGui::IsItemHovered())
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
		else
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
		draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	}


	/// @brief Reference: https://github.com/ocornut/imgui/issues/1901
	void CUI::toggle_button_ex(const String field_text, bool* v, const mint::fx::CColor& bg_active /*= MINT_GREEN_LIGHT()*/, const mint::fx::CColor& bg_inactive /*= MINT_GREY_LIGHT()*/, const mint::fx::CColor& fg_knob /*= MINT_WHITE()*/)
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = ImGui::GetFrameHeight();
		float width = height * 1.55f;
		float radius = height * 0.50f;

		ImGui::InvisibleButton(field_text.c_str(), ImVec2(width, height));
		if (ImGui::IsItemClicked())
			*v = !*v;

		float t = *v ? 1.0f : 0.0f;

		ImGuiContext& g = *GImGui;
		float ANIM_SPEED = 0.08f;
		if (g.LastActiveId == g.CurrentWindow->GetID(field_text.c_str()))// && g.LastActiveIdTimer < ANIM_SPEED)
		{
			float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
			t = *v ? (t_anim) : (1.0f - t_anim);
		}

		auto bginactivevec = bg_inactive.to_normalized_color_vec4();
		auto bgactivevec = bg_active.to_normalized_color_vec4();

		ImU32 col_bg;
		if (ImGui::IsItemHovered())
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(bginactivevec.r, bginactivevec.g, bginactivevec.b, bginactivevec.a - 0.2f), ImVec4(bgactivevec.r, bgactivevec.g, bgactivevec.b, bgactivevec.a - 0.2f), t));
		else
			col_bg = ImGui::GetColorU32(ImLerp(ImVec4(bginactivevec.r, bginactivevec.g, bginactivevec.b, bginactivevec.a + 0.2f), ImVec4(bgactivevec.r, bgactivevec.g, bgactivevec.b, bgactivevec.a + 0.2f), t));

		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
		draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(fg_knob.m_r, fg_knob.m_g, fg_knob.m_b, fg_knob.m_a));
	}

	void CUI::loading_bar(const String& field_id, f32 percent_value, const Vec2& size, const fx::CColor& background/*= MINT_GREY_DARK()*/, const fx::CColor& foreground/*= MINT_GREEN()*/)
	{
		auto bg = background.to_normalized_color_vec4();
		auto fg = foreground.to_normalized_color_vec4();
		f32 v = percent_value / 100.0f;

		ImGui::BufferingBar(field_id.c_str(), v, { size.x, size.y }, ImGui::GetColorU32(ImVec4(bg.r, bg.g, bg.b, bg.a)), ImGui::GetColorU32(ImVec4(fg.r, fg.g, fg.b, fg.a)));
	}

	void CUI::loading_circle(const String& field_id, f32 radius, f32 thikness, const fx::CColor& color /*= MINT_GREEN()*/)
	{
		auto col = color.to_normalized_color_vec4();

		ImGui::Spinner(field_id.c_str(), radius, thikness, ImGui::GetColorU32(ImVec4(col.r, col.g, col.b, col.a)));
	}

	bool CUI::edit_field_sint32(s32& value, s32 min, s32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/)
	{
		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			int prev = value;

			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth);
			ImGui::DragInt("s32", &prev, 1, min, max, "%d", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth);
			ImGui::InputInt("scalar", &prev, 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			value = prev;

			return true;
		}

		return false;
	}


	bool CUI::edit_field_sint64(s64& value, s64 min, s64 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/)
	{
		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			int prev = value;

			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth);
			ImGui::DragInt("s64", &prev, 1, min, max, "%d", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth);
			ImGui::InputInt("scalar", &prev, 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			value = prev;

			return true;
		}

		return false;
	}

	bool CUI::edit_field_uint32(u32& value, u32 min, u32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/)
	{
		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			int prev = value;

			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth);
			ImGui::DragInt("##u32", &prev, 1, min, max, "%d", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth);
			ImGui::InputInt("##scalar", &prev, 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			value = prev;

			return true;
		}

 		return false;
	}


	bool CUI::edit_field_uint64(u64& value, u64 min, u64 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/)
	{
		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			int prev = value;

			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth);
			ImGui::DragInt("u64", &prev, 1, min, max, "%d", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth);
			ImGui::InputInt("scalar", &prev, 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			value = prev;

			return true;
		}

		return false;
	}


	bool CUI::edit_field_f32(f32& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/, f32 speed)
	{
		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			float prev = value;

			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth);
			ImGui::DragFloat("f32", &prev, speed, min, max, "%.4f", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth);
			ImGui::InputFloat("scalar", &prev, 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			value = prev;

			return true;
		}

		return false;
	}


	bool CUI::edit_field_bool(bool& value, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/)
	{
		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			ImGui::PushID(slider_id);
			ImGui::Checkbox("boolean", &value);
			ImGui::PopID();
			help_marker_no_question_mark(field_desc);

			return true;
		}

		return false;
	}


	bool CUI::edit_field_vec2(Vec2& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/, f32 speed)
	{
		f32 prev[2]; prev[0] = value.x; prev[1] = value.y;

		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth /*250.0f*/);
			ImGui::DragFloat2("f32", prev, speed, min, max, "%.4f", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);


			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("X", &prev[0], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("Y", &prev[1], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			value.x = prev[0];
			value.y = prev[1];

			return true;
		}

		return false;
	}


	bool CUI::edit_field_vec2_ranged(Vec2& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/, f32 speed /*= 1.0f*/)
	{
		f32 prev[2]; prev[0] = value.x; prev[1] = value.y;

		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(2.0f * s_editDragFieldWidth /*250.0f*/);
		
			ImGui::DragFloatRange2("f32", &prev[0], &prev[1], speed, min, max, "Min: %.4f", "Max: %.4f", flags);
		
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);


			value.x = prev[0];
			value.y = prev[1];

			return true;
		}

		return false;
	}


	bool CUI::edit_field_vec3(Vec3& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/, f32 speed)
	{
		f32 prev[3]; prev[0] = value.x; prev[1] = value.y; prev[2] = value.z;

		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth /*250.0f*/);
			ImGui::DragFloat3("f32", prev, speed, min, max, "%.4f", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);


			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("X", &prev[0], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("Y", &prev[1], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("Z", &prev[2], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			value.x = prev[0];
			value.y = prev[1];
			value.z = prev[2];

			return true;
		}

		return false;
	}


	bool CUI::edit_field_vec4(Vec4& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/, f32 speed)
	{
		f32 prev[4]; prev[0] = value.x; prev[1] = value.y; prev[2] = value.z; prev[3] = value.w;

		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth /*250.0f*/);
			ImGui::DragFloat4("f32", prev, speed, min, max, "%.4f", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);


			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("X", &prev[0], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("Y", &prev[1], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("Z", &prev[2], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("W", &prev[3], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			value.x = prev[0];
			value.y = prev[1];
			value.z = prev[2];
			value.w = prev[3];

			return true;
		}

		return false;
	}


	bool CUI::edit_field_string(String& value, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/)
	{
		String text = "##" + field_text;

		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			u32 item_width = ImGui::CalcTextSize(value.c_str()).x;
			if (item_width < 75.0f) item_width = 75.0f;


			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(item_width);

			InputTextEx(text.c_str(), &value, ImGuiInputTextFlags_None, InputTextCallback, nullptr);

			ImGui::PopID();
			ImGui::PopItemWidth();

			help_marker_no_question_mark(field_desc);

			return true;
		}

		return false;
	}


	bool CUI::edit_field_color(fx::CColor& value, u8 min, u8 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/)
	{
		int prev[4]; prev[0] = value.m_r; prev[1] = value.m_g; prev[2] = value.m_b; prev[3] = value.m_a;

		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth /*250.0f*/);
			ImGui::DragInt4("u8", prev, 1.0f, min, max, "%d", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);


			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputInt("R", &prev[0], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputInt("G", &prev[1], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputInt("B", &prev[2], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputInt("A", &prev[3], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);


			value.set_color(SCAST(u8, prev[0]), SCAST(u8, prev[1]), SCAST(u8, prev[2]), SCAST(u8, prev[3]));

			return true;
		}

		return false;
	}


	bool CUI::edit_field_rect(CRect& value, f32 min, f32 max, const String& field_text, const String& field_desc, ImGuiID slider_id, ImGuiID scalar_id, ImGuiSliderFlags flags /*= ImGuiSliderFlags_None*/, f32 speed)
	{
		f32 prev[4]; prev[0] = value.get_x(); prev[1] = value.get_y(); prev[2] = value.get_width(); prev[3] = value.get_height();

		if (ImGui::CollapsingHeader(field_text.c_str()))
		{
			ImGui::PushID(slider_id);
			ImGui::PushItemWidth(s_editDragFieldWidth /*250.0f*/);
			ImGui::DragFloat4("f32", prev, speed, min, max, "%.4f", flags);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);


			ImGui::PushID(scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("X", &prev[0], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("Y", &prev[1], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("W", &prev[2], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			ImGui::SameLine();

			ImGui::PushID(++scalar_id);
			ImGui::PushItemWidth(s_editScalarFieldWidth /*125.0f*/);
			ImGui::InputFloat("H", &prev[3], 0, 0);
			ImGui::PopID();
			ImGui::PopItemWidth();
			help_marker_no_question_mark(field_desc);

			value.set_x(prev[0]);
			value.set_y(prev[1]);
			value.set_width(prev[2]);
			value.set_height(prev[3]);

			return true;
		}

		return false;
	}


	void CUI::help_marker(const String& desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * desc.size());
			ImGui::TextUnformatted(desc.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}


	void CUI::help_marker_no_question_mark(const String& desc)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * desc.size());
			ImGui::TextUnformatted(desc.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}


	bool CUI::ui_has_focus()
	{
		return ImGui::IsAnyItemHovered() || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	}


	s32 InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
			std::string* str = user_data->Str;
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		else if (user_data->ChainCallback)
		{
			// Forward to user callback, if any
			data->UserData = user_data->ChainCallbackUserData;
			return user_data->ChainCallback(data);
		}
		return 0;
	}


}