#include "UI.h"


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
		rlImGuiSetup(false);

		CFileystem fs(CFileystem::get_working_directory());

		if(fs.forward("EditorRessources") && fs.forward("Fonts"))
		{
			ImGuiIO& io = ImGui::GetIO();

			static const ImWchar ranges[] =
			{
			0x0020, 0x04ff, 0x0FFF,
			0,
			};
			ImFontConfig icons_config; icons_config.MergeMode = false; icons_config.PixelSnapH = true;
			
			String file = CFileystem::construct_from(fs.get_current_directory().as_string(), "DroidSans.ttf").as_string();

			auto font = io.Fonts->AddFontFromFileTTF(file.c_str(), 18.0f, &icons_config, ranges);

			rlImGuiReloadFonts();

			//ImGui::GetIO().FontDefault = font;

			return true;
		}
		
		MINT_LOG_CRITICAL("[{:.4f}][CUI::initialize] Failed loading fonts at \"{}\"!", MINT_APP_TIME, fs.get_current_directory().as_string());
		
		return false;
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
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
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