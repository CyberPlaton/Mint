#if MINT_DISTR
#else
#ifndef _MINTEDITOR_EDITOR_MAINMENUBAR_LAYER_H_
#define _MINTEDITOR_EDITOR_MAINMENUBAR_LAYER_H_


#include "../Common/Layer.h"
#include "Utility/Profiling/FunctionProfiler.h"


namespace mint::editor
{

	class CMainmenubarLayer : public CLayer
	{
	public:
		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		String get_layer_name() override final;
	};

}


#endif
#endif