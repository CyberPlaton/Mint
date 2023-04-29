#ifndef _MINTEDITOR_EDITOR_HIERARCHY_PANEL_LAYER_H_
#define _MINTEDITOR_EDITOR_HIERARCHY_PANEL_LAYER_H_


#include "../Common/Layer.h"


namespace mint::editor
{

	class CHierarchyPanelLayer : public CLayer
	{
	public:
		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		ImGuiWindowFlags get_flags() override final;

		String get_layer_name() override final;

	};

}


#endif