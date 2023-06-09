#if MINT_DISTR
#else
#include "EditorLayerLeftmostPanel.h"


namespace mint::editor
{


	bool CLeftmostPanelLayer::on_initialize()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CLeftmostPanelLayer::on_initialize");
		bool result = true;

		for (const auto& kid : get_children_layers())
		{
			result &= kid->on_initialize();
		}

		return result;
	}


	void CLeftmostPanelLayer::on_update(f32 dt)
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CLeftmostPanelLayer::on_update");
		CLayer::on_update(dt);
	}


	void CLeftmostPanelLayer::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CLeftmostPanelLayer::on_ui_frame");
		ImGui::SetNextWindowSize({ percent(get_window_width(), 25), percent(get_window_height(), 98) }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ 0.0f, percent(get_window_height(), 2.0f) }, ImGuiCond_Appearing);
		ImGui::Begin("Leftmost Panel", &m_enabled, get_flags());

		CLayer::on_ui_frame();

		ImGui::End();
	}


	mint::String CLeftmostPanelLayer::get_layer_name()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CLeftmostPanelLayer::get_layer_name");
		return "CLeftmostPanelLayer";
	}


	ImGuiWindowFlags CLeftmostPanelLayer::get_flags()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CLeftmostPanelLayer::get_flags");
		return ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	}


}
#endif