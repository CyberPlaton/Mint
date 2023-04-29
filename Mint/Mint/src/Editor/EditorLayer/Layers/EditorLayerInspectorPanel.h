#ifndef _MINTEDITOR_EDITOR_INSPECTOR_PANEL_LAYER_H_
#define _MINTEDITOR_EDITOR_INSPECTOR_PANEL_LAYER_H_


#include "../Common/Layer.h"


namespace mint::editor
{

	class CInspectorPanelLayer : public CLayer
	{
	public:
		bool on_initialize() override final;

		bool is_enabled() { return m_enabled; };

		void set_is_enabled(bool enabled) { m_enabled = enabled; };

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		String get_layer_name() override final;

	private:
		bool m_enabled = true;
	};

}


#endif