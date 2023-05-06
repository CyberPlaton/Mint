#if MINT_DISTR
#else
#include "EditorLayerViewportPanel.h"


namespace mint::editor
{

	bool CViewportPanelLayer::on_initialize()
	{
		return true;
	}


	void CViewportPanelLayer::on_update(f32 dt)
	{

	}


	void CViewportPanelLayer::on_ui_frame()
	{
	}


	mint::String CViewportPanelLayer::get_layer_name()
	{
		return "Viewport Panel";
	}


}
#endif