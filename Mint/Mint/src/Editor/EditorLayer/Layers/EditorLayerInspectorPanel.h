#if MINT_DISTR
#else
#ifndef _MINTEDITOR_EDITOR_INSPECTOR_PANEL_LAYER_H_
#define _MINTEDITOR_EDITOR_INSPECTOR_PANEL_LAYER_H_


#include "../Common/Layer.h"
#include "../../ComponentEditor/ComponentEditorStack.h"


namespace mint::editor
{

	class CInspectorPanelLayer : public CLayer
	{
	public:
		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		String get_layer_name() override final;

		ImGuiWindowFlags get_flags() override final;

	private:
		CComponentEditorStack m_componentEditorStack;

		bool m_addingComponent = false;

	private:
		void show_main_frame();

		void show_add_component_dialog();
	};

}


#endif
#endif