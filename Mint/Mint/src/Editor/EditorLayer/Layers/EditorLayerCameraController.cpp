#include "EditorLayerCameraController.h"


namespace mint::editor
{


	CCameraControllerLayer::CCameraControllerLayer(mint::fx::CCamera2D* editor_camera) :
		m_camera(nullptr)
	{
		m_camera = editor_camera;
	}


	void CCameraControllerLayer::on_update(f32 dt)
	{

	}


	bool CCameraControllerLayer::on_initialize()
	{
		return true;
	}


	mint::String CCameraControllerLayer::get_layer_name()
	{
		return "Editor Camera Controller";
	}


}