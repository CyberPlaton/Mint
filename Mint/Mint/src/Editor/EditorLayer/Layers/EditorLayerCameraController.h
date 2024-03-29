#if MINT_DISTR
#else
#ifndef _MINTEDITOR_EDITOR_CAMERA_CONTROLLER_LAYER_H_
#define _MINTEDITOR_EDITOR_CAMERA_CONTROLLER_LAYER_H_


#include "../Common/Layer.h"
#include "Graphics/Common/Camera2D.h"
#include "Utility/Input/Input.h"
#include "Common/Timestep.h"
#include "Utility/Profiling/FunctionProfiler.h"


namespace mint::editor
{

	class CCameraControllerLayer : public CLayer
	{
	public:
		CCameraControllerLayer();

		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		String get_layer_name() override final;

	private:

	};

}

#endif
#endif