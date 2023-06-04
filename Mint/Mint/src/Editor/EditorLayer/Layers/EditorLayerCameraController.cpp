#if MINT_DISTR
#else
#include "EditorLayerCameraController.h"


namespace mint::editor
{


	CCameraControllerLayer::CCameraControllerLayer()
	{
	}


	void CCameraControllerLayer::on_update(f32 dt)
	{
		if (mint::CUI::ui_has_focus()) return;

		dt = mint::CTimestep::get_real_frametime(); // Ignore editor frametime, as we pause it during edit.
		f32 speed = GlobalData::Get().s_DefaultEditorCameraSpeed;
		f32 zoom_speed = GlobalData::Get().s_DefaultEditorCameraZoomSpeed;
		auto camera = mint::fx::CCameraManager::Get().get_active_camera();

		if (CInput::is_key_held_enum(KeyboardKey::KEY_LEFT_SHIFT))
		{
			speed = GlobalData::Get().s_DefaultEditorCameraSpeedDecreased;
			zoom_speed = GlobalData::Get().s_DefaultEditorCameraZoomSpeedDecreased;
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_A))
		{
			camera->translate({ -dt * speed, 0.0f });
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_D))
		{
			camera->translate({ dt * speed, 0.0f });
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_W))
		{
			camera->translate({ 0.0f, -dt * speed });
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_S))
		{
			camera->translate({ 0.0f, dt * speed });
		}


		if (CInput::is_mouse_button_held(MOUSE_BUTTON_MIDDLE))
		{
			auto delta = GetMouseDelta();
			auto zoom = 1.0f / camera->get_zoom();
			camera->translate({ -delta.x * zoom, -delta.y * zoom });
		}


		if (f32 wheel_delta = CInput::get_mouse_wheel(); wheel_delta != 0.0f)
		{
			wheel_delta *= zoom_speed;

			camera->zoom(wheel_delta);
		}

	}


	bool CCameraControllerLayer::on_initialize()
	{
		return true;
	}


	mint::String CCameraControllerLayer::get_layer_name()
	{
		return "CCameraControllerLayer";
	}


	void CCameraControllerLayer::on_ui_frame()
	{
	}


}
#endif