#if MINT_DISTR
#else
#include "EditorLayerViewportPanel.h"


namespace mint::editor
{

	bool CViewportPanelLayer::on_initialize()
	{
		return true;
	}


	void CViewportPanelLayer::on_update(f32 dt)
	{
	}


	void CViewportPanelLayer::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CViewportPanelLayer::on_ui_frame");

		ImGui::SetNextWindowSize({ percent(get_window_width(), 50), percent(get_window_height(), 5.5f) }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ percent(get_window_width(), 25), percent(get_window_height(), 2.0f) }, ImGuiCond_Appearing);
		ImGui::Begin("Viewport Panel", &m_enabled, get_flags());


		ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.9f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

		ImGui::SameLine();

		if(ImGui::SmallButton(ICON_FA_DISPLAY)) { ImGui::OpenPopup("Viewport Window Options Popup"); }
		CUI::help_marker_no_question_mark("Edit window settings");

		window_options();

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_VIDEO)) { ImGui::OpenPopup("Viewport Camera Options Popup"); }
		CUI::help_marker_no_question_mark("Edit camera settings");

		camera_options();

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_TABLE_CELLS)) { ImGui::OpenPopup("Viewport Grid Options Popup"); }
		CUI::help_marker_no_question_mark("Edit grid settings");

		grid_options();

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_BUG)) { ImGui::OpenPopup("Viewport Debug Render Options Popup"); }
		CUI::help_marker_no_question_mark("Edit debug rendering settings");

		debug_render_options();

		ImGui::PopStyleColor(2);

		main_frame();

		ImGui::End();
	}


	mint::String CViewportPanelLayer::get_layer_name()
	{
		return "CViewportPanelLayer";
	}


	void CViewportPanelLayer::on_frame()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CViewportPanelLayer::on_frame");

		if(MINT_ENGINE()->is_in_editor_mode())
		{
			auto dr = fx::CRenderingPassStack::Get().get_rendering_pass_as< fx::CDebugRenderer >("CDebugRenderer");
			auto wqr = fx::CRenderingPassStack::Get().get_rendering_pass_as< CWorldQueryDebugRender >("CWorldQueryDebugRender");


			// World Query debug rendering.
			if (GlobalData::Get().s_EditorDebugRenderAABBs)
			{
				wqr->set_render_aabbs(true);
				wqr->set_aabb_color(GlobalData::Get().s_EditorWorldQueryAABBColor);
			}
			else wqr->set_render_aabbs(false);

			if (GlobalData::Get().s_EditorDebugRenderAABBFullInformation) wqr->set_render_full_information(true);
			else wqr->set_render_full_information(false);



			// General debug rendering.
			if (GlobalData::Get().s_EditorDebugRenderDestinationRect) dr->set_render_destination_rectangle(true);
			else dr->set_render_destination_rectangle(false);

			if (GlobalData::Get().s_EditorDebugRenderOriginPoint) dr->set_render_sprite_origin(true);
			else dr->set_render_sprite_origin(false);
			
 			if(GlobalData::Get().s_EditorDebugRenderAll)
 			{
				dr->set_render_all_entities(true);
				dr->clear_entity_filter();

				wqr->set_render_all_entities(true);
				wqr->clear_entity_filter();
 			}
 			else
 			{
				dr->set_render_all_entities(false);
				wqr->set_render_all_entities(false);

 				if(GlobalData::Get().s_EditorInspectedEntity != entt::null)
 				{
					dr->add_entity_to_filter(GlobalData::Get().s_EditorInspectedEntity);
					wqr->add_entity_to_filter(GlobalData::Get().s_EditorInspectedEntity);
 				}
 			} 
		}
	}


	void CViewportPanelLayer::main_frame()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Viewport"))
			{
				if (ImGui::MenuItem("Item"))
				{
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}


	ImGuiWindowFlags CViewportPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
			 | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
			 | ImGuiWindowFlags_NoScrollbar;
	}


	void CViewportPanelLayer::window_options()
	{
		if (ImGui::BeginPopup("Viewport Window Options Popup"))
		{
			ImGui::SeparatorText("Window Settings");
			
			ImGui::SameLine();

			CUI::help_marker("Please note that changing the window dimension is done in the application \"Manifest\"!");

			auto& window = MINT_ENGINE()->get_main_window_ref();

			auto size = window.get_size();
			auto max_size = window.get_max_possible_window_size();
			auto fullscreen = window.is_fullscreen();

			ImGui::Text(TextFormat("Size: {%.1f:%.1f}", size.x, size.y));
			ImGui::Text(TextFormat("Max Size: {%.1f:%.1f}", max_size.x, max_size.y));
			if(ImGui::Checkbox("Fullscreen", &fullscreen))
			{
				window.set_is_fullscreen(fullscreen);
			}
			
			if (ImGui::CollapsingHeader("Anti-Aliasing"))
			{
				auto fxaa = mint::fx::CRenderingPassStack::Get().get_rendering_pass_as < mint::fx::CFXAA >("CFXAA");
				auto threshold = fxaa->get_blur_threshold();
				auto saturation = fxaa->get_saturation();
				auto rtOffset = fxaa->get_render_texture_offset();

				ImGuiID sid = 10000;
				ImGuiID scid = 20000;

				CUI::edit_field_f32(threshold, 0.0f, 1.0f, "FXAA Threshold", "", sid++, scid++, ImGuiSliderFlags_Logarithmic, 0.01f);
				CUI::edit_field_f32(saturation, 0.0f, 1.0f, "FXAA Saturation", "", sid++, scid++, ImGuiSliderFlags_Logarithmic, 0.01f);
				CUI::edit_field_f32(rtOffset, 0.0f, 1.0f, "FXAA RT Offset", "", sid++, scid++, ImGuiSliderFlags_Logarithmic, 0.01f);

				fxaa->set_blur_threshold(threshold);
				fxaa->set_saturation(saturation);
				fxaa->set_render_texture_offset(rtOffset);


			}
			ImGui::EndPopup();
		}
	}


	void CViewportPanelLayer::camera_options()
	{
		if (ImGui::BeginPopup("Viewport Camera Options Popup"))
		{
			ImGui::SeparatorText("Editor Camera Settings");



			ImGui::EndPopup();
		}
	}


	void CViewportPanelLayer::grid_options()
	{
		if (ImGui::BeginPopup("Viewport Grid Options Popup"))
		{
			ImGui::SeparatorText("Grid Settings");



			ImGui::EndPopup();
		}
	}


	void CViewportPanelLayer::debug_render_options()
	{
		if (ImGui::BeginPopup("Viewport Debug Render Options Popup"))
		{
			ImGui::SeparatorText("Debug Render Settings");

			ImGui::Checkbox("Render All", &GlobalData::Get().s_EditorDebugRenderAll);
			GlobalData::Get().s_EditorDebugRenderSelected = !GlobalData::Get().s_EditorDebugRenderAll;

			ImGui::SameLine();

			CUI::help_marker("Change whether to draw debug information for all game objects or for the inspected one only!");

			ImGui::Checkbox("Destination Rectangle", &GlobalData::Get().s_EditorDebugRenderDestinationRect);
			ImGui::Checkbox("Origin Point", &GlobalData::Get().s_EditorDebugRenderOriginPoint);

			ImGui::Checkbox("World Query AABB", &GlobalData::Get().s_EditorDebugRenderAABBs);
			ImGui::Checkbox("World Query Full Information", &GlobalData::Get().s_EditorDebugRenderAABBFullInformation);

			CUI::edit_field_vec4(GlobalData::Get().s_EditorWorldQueryAABBColor, 0.0f, 255.0f, "AABB Color", "", 10000, 20000);

			ImGui::EndPopup();
		}

	}


}
#endif