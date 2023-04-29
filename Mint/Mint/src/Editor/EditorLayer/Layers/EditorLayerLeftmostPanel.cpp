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
		CLayer::on_ui_frame();
	}


	mint::String CLeftmostPanelLayer::get_layer_name()
	{
		return "Leftmost Panel";
	}


	ImGuiWindowFlags CLeftmostPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


}