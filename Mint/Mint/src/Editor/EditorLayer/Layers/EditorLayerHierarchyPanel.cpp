#include "EditorLayerHierarchyPanel.h"


namespace mint::editor
{


	bool CHierarchyPanelLayer::on_initialize()
	{
		return true;
	}


	void CHierarchyPanelLayer::on_update(f32 dt)
	{

	}


	void CHierarchyPanelLayer::on_ui_frame()
	{
		ImGui::SetNextWindowSize({ percent(get_window_width(), 30), percent(get_window_height(), 75) }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos({ 0.0f, 20.0f }, ImGuiCond_FirstUseEver);
		ImGui::Begin("Hierarchy", &m_enabled, get_flags());

		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("Hierarchy"))
		{
			if (ImGui::BeginMenu("Create entity"))
			{
				ImGui::EndMenu();
			}
			if(ImGui::MenuItem("Create empty entity"))
			{

			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();


		ImGui::End();
	}


	mint::String CHierarchyPanelLayer::get_layer_name()
	{
		return "Scene Hierarchy Panel";
	}


	ImGuiWindowFlags CHierarchyPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


}