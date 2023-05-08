#include "Application.h"


namespace mint
{

#if MINT_DISTR
	bool CApplication::initialize_application(const String& manifest_filepath)
	{
		if(initialize(manifest_filepath))
		{
			return true;
		}

		return false;
	}


	void CApplication::terminate_application()
	{
		terminate();
	}


	void CApplication::run()
	{
		while (is_running())
		{
			on_before_update();

			on_update(MINT_ENGINE()->get_engine_frametime());


			begin_rendering();



			begin_frame();

			frame();

			end_frame();



			ui_frame_begin();

			ui_frame_render();

			ui_frame_end();



			end_rendering();



			on_after_update(MINT_ENGINE()->get_engine_frametime());
		}
	}


#else

	bool CEditor::initialize_editor(const String& manifest_filepath)
	{
		if (initialize(manifest_filepath))
		{
			IService::print_registered_services();


			get_main_window_const().print_window_configurations();


			set_engine_fps(60.0f);
			set_engine_window_title("Mint Engine Editor");


			m_editorCamera = new mint::fx::CCamera2D({ 255, 150, 50, 255 }, 0, 0, MINT_ENGINE()->get_main_window_const().get_w(), MINT_ENGINE()->get_main_window_const().get_h());

			
			bool result = true;

			result &= editor::CEditorIconManager::Get().initialize();
			result &= create_layer_stack();


			if(result) m_layerStack.print_registered_layers();


			register_engine_components_to_reflection_system();

			editor::GlobalData::Get().s_ComponentDatabase.print_registered_components();


			return result;
		}

		return false;
	}


	void CEditor::terminate_editor()
	{
		m_layerStack.clear_all_layers();

		delete m_editorCamera; m_editorCamera = nullptr;

		terminate();
	}


	void CEditor::run()
	{
		while (is_running())
		{
			on_editor_before_update();

			on_editor_update(MINT_ENGINE()->get_engine_frametime());


			begin_rendering();



			on_editor_frame_begin();

			on_editor_frame_render();

			on_editor_frame_end();



			on_editor_ui_frame_begin();

			on_editor_ui_frame_render();

			on_editor_ui_frame_end();



			end_rendering();



			on_editor_after_update(MINT_ENGINE()->get_engine_frametime());
		}
	}


	void CEditor::on_editor_ui_frame_begin()
	{
		ui_frame_begin();

		ImGui::ShowDemoWindow();
	}


	void CEditor::on_editor_ui_frame_render()
	{
		ui_frame_render();

		if (!m_editingMode) return;

		m_layerStack.on_ui_frame();
	}


	void CEditor::on_editor_ui_frame_end()
	{
		ui_frame_end();
	}


	void CEditor::on_editor_before_update()
	{
		on_before_update();

		m_layerStack.on_before_update();
	}


	void CEditor::on_editor_update(f32 dt)
	{
		if (CInput::is_key_pressed_enum(KEY_ESCAPE))
		{
			if(editor::GlobalData::Get().s_EditorInspectedEntity != entt::null)
			{
				editor::GlobalData::Get().s_EditorInspectedEntity = entt::null;
				return;
			}
			
			toggle(m_editingMode);

			if (m_editingMode)
			{
				auto viewport = get_layer_as< editor::CViewportPanelLayer >("Viewport Panel");

				viewport->set_is_enabled(true);

				set_engine_fps(0.0f);

				set_engine_window_title("Mint Engine Editor: Editing Mode");

				MINT_ACTIVE_SCENE()->push_camera(m_editorCamera);
			}
			else
			{
				auto viewport = get_layer_as< editor::CViewportPanelLayer >("Viewport Panel");

				viewport->set_is_enabled(false);

				set_engine_fps(30.0f);

				set_engine_window_title("Mint Engine Editor");

				MINT_ACTIVE_SCENE()->pop_camera();
			}
		}

		on_update(dt);

		m_layerStack.on_update(dt);
	}


	void CEditor::on_editor_after_update(f32 dt)
	{
		on_after_update(dt);

		m_layerStack.on_after_update(dt);
	}


	void CEditor::on_editor_frame_begin()
	{
		begin_frame();

		if (m_editingMode)
		{
			rlPushMatrix();
				rlTranslatef(0, 0, 0);
				rlRotatef(-90, 1, 0, 0);
				DrawGrid(1000, editor::GlobalData::Get().s_DefaultGridcellSize);
			rlPopMatrix();
		}
	}


	void CEditor::on_editor_frame_render()
	{
		frame();

		m_layerStack.on_frame();
	}


	void CEditor::on_editor_frame_end()
	{
		end_frame();
	}


	bool CEditor::create_layer_stack()
	{
		mint::String failed_on;
		mint::u32 failed_count = 0;
		bool failed = false;

		if(!m_layerStack.try_push_layer(new editor::CCameraControllerLayer(m_editorCamera)))
		{
			if(failed_on.empty()) failed_on = "CCameraControllerLayer";
			failed_count++;
			failed = true;
		}

		auto rightmost_panel = new editor::CRightmostPanelLayer();
		rightmost_panel->add_child_layer(new editor::CInspectorPanelLayer());
		if (!m_layerStack.try_push_layer(rightmost_panel))
		{
			if (failed_on.empty()) failed_on = "CRightmostPanelLayer";
			failed_count++;
			failed = true;
		}

		auto leftmost_panel = new editor::CLeftmostPanelLayer();
		leftmost_panel->add_child_layer(new editor::CHierarchyPanelLayer());
		leftmost_panel->add_child_layer(new editor::CProjectAssetsPanelLayer());
		if (!m_layerStack.try_push_layer(leftmost_panel))
		{
			if (failed_on.empty()) failed_on = "CLeftmostPanelLayer";
			failed_count++;
			failed = true;
		}
		
		if (!m_layerStack.try_push_layer(new editor::CViewportPanelLayer()))
		{
			if (failed_on.empty()) failed_on = "CViewportPanelLayer";
			failed_count++;
			failed = true;
		}
		if (!m_layerStack.try_push_layer(new editor::CMainmenubarLayer()))
		{
			if (failed_on.empty()) failed_on = "CMainmenubarLayer";
			failed_count++;
			failed = true;
		}
		

		if(failed)
		{
			MINT_LOG_CRITICAL("[{:.4f}][CEditor::create_layer_stack] Failed creating \"{}\" editor layers, first to fail was \"{}\"!", MINT_APP_TIME, failed_count, failed_on);
		}

		return !failed;
	}


	void CEditor::register_engine_components_to_reflection_system()
	{
		mint::component::SIdentifier id;
		mint::component::SSceneHierarchy h;
		mint::component::SRigidBody rb;
		mint::component::STransform t;
		mint::component::SSprite s;
		mint::component::SAnimatedSprite as;
		mint::component::SScript sc;
	}


	void CEditor::toggle(bool& value)
	{
		value = !value;
	}


#endif
}