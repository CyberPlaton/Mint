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
		auto width = ImGui::GetWindowWidth();
		auto height = ImGui::GetWindowHeight();

		ImGui::BeginChild("Hierarchy", { width, percent(height, 50) }, true, get_flags());

		if(ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu(lang::get_word(lang::Word_Hierarchy)))
			{
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();

		ImGui::EndChild();
	}


	mint::String CHierarchyPanelLayer::get_layer_name()
	{
		return "Scene Hierarchy Panel";
	}


	ImGuiWindowFlags CHierarchyPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


}