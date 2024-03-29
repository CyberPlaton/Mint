#if MINT_DISTR
#else
#include "EditorLayerRightmostPanel.h"


namespace mint::editor
{
	bool CRightmostPanelLayer::on_initialize()
	{
		bool result = true;

		for (const auto& kid : get_children_layers())
		{
			result &= kid->on_initialize();
		}

		return result;
	}


	void CRightmostPanelLayer::on_update(f32 dt)
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CRightmostPanelLayer::on_update");

		CLayer::on_update(dt);
	}


	void CRightmostPanelLayer::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CRightmostPanelLayer::on_ui_frame");

		ImGui::SetNextWindowSize({ percent(get_window_width(), 25), percent(get_window_height(), 98) }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() - percent(get_window_width(), 25), GlobalData::Get().s_EditorMainMenubarHeight }, ImGuiCond_Appearing);
		ImGui::Begin("Rightmost Panel", &m_enabled, get_flags());

		CLayer::on_ui_frame();

		ImGui::End();
	}


	mint::String CRightmostPanelLayer::get_layer_name()
	{
		return "CRightmostPanelLayer";
	}


	ImGuiWindowFlags CRightmostPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	}
}
#endif