#ifndef _MINTEDITOR_EDITOR_VIEWPORT_PANEL_LAYER_H_
#define _MINTEDITOR_EDITOR_VIEWPORT_PANEL_LAYER_H_


#include "../Common/Layer.h"
#include "../../RessourceManagement/EditorIconManager.h"


namespace mint::editor
{

	class CViewportPanelLayer : public CLayer
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