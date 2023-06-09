#if MINT_DISTR
#else
#ifndef _MINTEDITOR_EDITOR_LEFTMOST_PANEL_LAYER_H_
#define _MINTEDITOR_EDITOR_LEFTMOST_PANEL_LAYER_H_


#include "../Common/Layer.h"
#include "Utility/Profiling/FunctionProfiler.h"


namespace mint::editor
{

	class CLeftmostPanelLayer : public CLayer
	{
	public:
		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		String get_layer_name() override final;

		ImGuiWindowFlags get_flags() override final;

	};

}


#endif
#endif