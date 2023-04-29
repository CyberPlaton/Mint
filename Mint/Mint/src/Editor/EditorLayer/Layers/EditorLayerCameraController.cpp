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
		if (mint::CUI::ui_has_focus()) return;


		dt = mint::CTimestep::get_real_frametime(); // Ignore editor frametime, as we pause it during edit.
		f32 speed = editor::s_DefaultEditorCameraSpeed;
		f32 zoom_speed = editor::s_DefaultEditorCameraZoomSpeed;

		if (CInput::is_key_held_enum(KeyboardKey::KEY_LEFT_SHIFT))
		{
			speed = editor::s_DefaultEditorCameraSpeedDecreased;
			zoom_speed = editor::s_DefaultEditorCameraZoomSpeedDecreased;
		}
		if(CInput::is_key_held_enum(KeyboardKey::KEY_A))
		{
			m_camera->translate({ -dt * speed, 0.0f });
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_D))
		{
			m_camera->translate({ dt * speed, 0.0f });
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_W))
		{
			m_camera->translate({ 0.0f, -dt * speed });
		}
		if (CInput::is_key_held_enum(KeyboardKey::KEY_S))
		{
			m_camera->translate({ 0.0f, dt * speed });
		}


		if(CInput::is_mouse_button_held(MOUSE_BUTTON_MIDDLE))
		{
			auto delta = GetMouseDelta();
			auto zoom = 1.0f / m_camera->get_zoom();
			m_camera->translate({ -delta.x * zoom, -delta.y * zoom });
		}


		if(f32 wheel_delta = CInput::get_mouse_wheel(); wheel_delta != 0.0f)
		{
			wheel_delta *= zoom_speed;

			m_camera->zoom(wheel_delta);
		}

	}


	bool CCameraControllerLayer::on_initialize()
	{
		return true;
	}


	mint::String CCameraControllerLayer::get_layer_name()
	{
		return "Editor Camera Controller";
	}


	void CCameraControllerLayer::on_ui_frame()
	{
		const char* text = TextFormat("Zoom: %.3f", m_camera->get_zoom());

		DrawText(text, 1, 20, 24, RAYWHITE);
	}


}