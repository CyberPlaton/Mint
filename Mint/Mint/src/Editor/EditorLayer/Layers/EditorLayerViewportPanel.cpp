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
		ImGui::SetNextWindowSize({ percent(get_window_width(), 50), percent(get_window_height(), 5.5f) }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ percent(get_window_width(), 25), percent(get_window_height(), 2.0f) }, ImGuiCond_Appearing);
		ImGui::Begin("Viewport Panel", &m_enabled, get_flags());


		ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.9f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

		ImGui::SameLine();

		if(ImGui::SmallButton(ICON_FA_DISPLAY)) { m_showWindowOptions = true; }
		CUI::help_marker_no_question_mark("Edit window settings");


		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_VIDEO)) { m_showCameraOptions = true; }
		CUI::help_marker_no_question_mark("Edit camera settings");


		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_TABLE_CELLS)) { m_showGridOptions = true; }
		CUI::help_marker_no_question_mark("Edit grid settings");
		
		ImGui::PopStyleColor(2);

		main_frame();

		ImGui::End();

		if (m_showWindowOptions) window_options();
		if (m_showCameraOptions) camera_options();
		if (m_showGridOptions) grid_options();
	}


	mint::String CViewportPanelLayer::get_layer_name()
	{
		return "CViewportPanelLayer";
	}


	void CViewportPanelLayer::on_frame()
	{
		if(MINT_ENGINE()->is_in_editor_mode())
		{
			auto& entities = CSAS::Get().retrieve_visible_entities();

			fx::CDebugRenderer::on_render(entities);
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

	}


	void CViewportPanelLayer::camera_options()
	{

	}


	void CViewportPanelLayer::grid_options()
	{

	}


}
#endif