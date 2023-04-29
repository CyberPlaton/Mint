#include "EditorLayerInspectorPanel.h"


namespace mint::editor
{


	bool CInspectorPanelLayer::on_initialize()
	{
		return true;
	}


	void CInspectorPanelLayer::on_update(f32 dt)
	{

	}


	void CInspectorPanelLayer::on_ui_frame()
	{

	}


	mint::String CInspectorPanelLayer::get_layer_name()
	{
		return "Entity Inspector Panel";
	}


}