#ifndef _MINTEDITOR_EDITOR_CAMERA_CONTROLLER_LAYER_H_
#define _MINTEDITOR_EDITOR_CAMERA_CONTROLLER_LAYER_H_


#include "../Common/Layer.h"
#include "Graphics/Common/Camera2D.h"


namespace mint::editor
{

	class CCameraControllerLayer : public CLayer
	{
	public:
		CCameraControllerLayer(mint::fx::CCamera2D* editor_camera);

		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		String get_layer_name() override final;

	private:
		mint::fx::CCamera2D* m_camera;

	};

}

#endif