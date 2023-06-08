#if MINT_DISTR
#else
#ifndef _MINTEDITOR_EDITOR_PROFILER_PANEL_LAYER_H_
#define _MINTEDITOR_EDITOR_PROFILER_PANEL_LAYER_H_


#include "../Common/Layer.h"
#include "Utility/Profiling/FunctionProfiler.h"

namespace mint::editor
{

	class CProfilerPanelLayer : public CLayer
	{
	public:
		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		void on_frame() override final;

		String get_layer_name() override final;

		ImGuiWindowFlags CProfilerPanelLayer::get_flags() override final;

	private:

	private:
		void main_frame();

	};

}

#endif
#endif