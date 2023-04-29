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

	}


	mint::String CHierarchyPanelLayer::get_layer_name()
	{
		return "Scene Hierarchy Panel";
	}


}