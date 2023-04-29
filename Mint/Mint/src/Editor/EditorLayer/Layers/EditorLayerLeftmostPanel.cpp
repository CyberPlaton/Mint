#include "EditorLayerLeftmostPanel.h"


namespace mint::editor
{


	bool CLeftmostPanelLayer::on_initialize()
	{
		bool result = true;

		for(const auto& kid : get_children_layers())
		{
			result &= kid->on_initialize();
		}

		return result;
	}


	void CLeftmostPanelLayer::on_update(f32 dt)
	{
		CLayer::on_update(dt);
	}


	void CLeftmostPanelLayer::on_ui_frame()
	{
		ImGui::SetNextWindowSize({ percent(get_window_width(), 30), percent(get_window_height(), 85) }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos({ 0.0f, percent(get_window_height(), 2.5f) }, ImGuiCond_Appearing);
		ImGui::Begin("Leftmost Panel", &m_enabled, get_flags());

		CLayer::on_ui_frame();

		ImGui::End();
	}


	mint::String CLeftmostPanelLayer::get_layer_name()
	{
		return "Leftmost Panel";
	}


	ImGuiWindowFlags CLeftmostPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	}


}