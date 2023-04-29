#include "EditorLayerProjectAssetsPanel.h"


namespace mint::editor
{


	bool CProjectAssetsPanelLayer::on_initialize()
	{
		return true;
	}


	void CProjectAssetsPanelLayer::on_update(f32 dt)
	{

	}


	void CProjectAssetsPanelLayer::on_ui_frame()
	{
		auto width = ImGui::GetWindowWidth();
		auto height = ImGui::GetWindowHeight();

		
		ImGui::BeginChild("Project Assets", { width, percent(height, 30) }, true, get_flags());

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Project Assets"))
			{
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();

		ImGui::EndChild();
	}


	mint::String CProjectAssetsPanelLayer::get_layer_name()
	{
		return "Project Assets Panel";
	}


	ImGuiWindowFlags CProjectAssetsPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


}