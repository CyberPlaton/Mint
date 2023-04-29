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
		ImGui::SetNextWindowSize({ percent(get_window_width(), 30), percent(get_window_height(), 25) }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos({ 0.0f, percent(get_window_height(), 75) }, ImGuiCond_FirstUseEver);
		ImGui::Begin("Project Assets", &m_enabled, get_flags());

		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("Project Assets"))
		{
		}
		ImGui::EndMenuBar();


		ImGui::End();
	}


	mint::String CProjectAssetsPanelLayer::get_layer_name()
	{
		return "Project Assets Panel";
	}


	ImGuiWindowFlags CProjectAssetsPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


}