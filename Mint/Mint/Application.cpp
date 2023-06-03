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
			set_engine_window_title("Mint Engine " MINT_ENGINE_VERSION_STRING);

			mint::fx::CColor color = { 255, 150, 50, 255 };

			mint::fx::CCameraManager::Get().push_camera< mint::fx::CEditorCamera >("EditorCamera", color, 0, 0, MINT_ENGINE()->get_main_window_const().get_w(), MINT_ENGINE()->get_main_window_const().get_h());

			
			bool result = true;

			result &= editor::CEditorIconManager::Get().initialize();
			result &= create_layer_stack();


			if(result) m_layerStack.print_registered_layers();


			register_engine_components_to_reflection_system();

			editor::GlobalData::Get().s_ComponentDatabase.print_registered_components();

			MINT_REGISTER_EVENT_LISTENER(SSceneChangeEventDelegate);

			m_timer.start_timer();

			return result;
		}

		return false;
	}


	void CEditor::terminate_editor()
	{
		m_layerStack.clear_all_layers();

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


	bool CEditor::is_in_editor_mode()
	{
		return m_editingMode;
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
				set_editing_mode();
			}
			else
			{
				unset_editing_mode();
			}
		}
		if(m_sceneReloaded && !CSceneManager::Get().is_transitioning())
		{
			// Reloading can be done in editing mode only, thus no need to toggle anything, just recreate the camera.
			mint::fx::CColor color = { 255, 150, 50, 255 };

			mint::fx::CCameraManager::Get().push_camera< mint::fx::CCamera2D >("EditorCamera", color, 0, 0, MINT_ENGINE()->get_main_window_const().get_w(), MINT_ENGINE()->get_main_window_const().get_h());

			m_sceneReloaded = false;
		}



		on_update(dt);

		m_layerStack.on_update(dt);
	}


	void CEditor::on_editor_after_update(f32 dt)
	{
		on_after_update(dt);

		m_layerStack.on_after_update(dt);


		if(m_timer.seconds_elapsed() > 5)
		{
			auto fps = CTimestep::get_real_fps();

			if (fps < 60.0f)
			{
				MINT_LOG_WARN("[{:.4f}][CEditor::on_editor_after_update] Low FPS: \"{}\"!", MINT_APP_TIME, fps);
			}
			else if (fps < 30.0f)
			{
				MINT_LOG_ERROR("[{:.4f}][CEditor::on_editor_after_update] Very Low FPS: \"{}\"!", MINT_APP_TIME, fps);
			}

			m_timer.start_timer();
		}
	}


	void CEditor::on_editor_frame_begin()
	{
		auto dr = fx::CRenderingPassStack::Get().get_rendering_pass_as< fx::CDebugRenderer >("CDebugRenderer");

		if (m_editingMode)
		{
			dr->set_render_grid(true, editor::GlobalData::Get().s_DefaultGridcellSize, 1000);
		}
		else
		{
			dr->set_render_grid(false);
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


	void CEditor::print_engine_context()
	{
		MINT_LOG_INFO("Runtime version: \"{}\", Editor version: \"{}\"", MINT_ENGINE_VERSION_STRING, MINT_EDITOR_VERSION_STRING);
		MINT_LOG_SEPARATOR();
	}


	bool CEditor::create_layer_stack()
	{
		mint::String failed_on;
		mint::u32 failed_count = 0;
		bool failed = false;

		// Create the root layer.
		auto root = new editor::CRootLayer();
		m_layerStack.try_push_layer(root, true);



		// Create the hierarchy of layers.
		auto camera_controller = new editor::CCameraControllerLayer();
		root->add_child_layer(camera_controller);
		if(!m_layerStack.try_push_layer(camera_controller))
		{
			if(failed_on.empty()) failed_on = "CCameraControllerLayer";
			failed_count++;
			failed = true;
		}


		auto rightmost_panel = new editor::CRightmostPanelLayer();
		root->add_child_layer(rightmost_panel);
		rightmost_panel->add_child_layer(new editor::CInspectorPanelLayer());
		if (!m_layerStack.try_push_layer(rightmost_panel))
		{
			if (failed_on.empty()) failed_on = "CRightmostPanelLayer";
			failed_count++;
			failed = true;
		}


		auto leftmost_panel = new editor::CLeftmostPanelLayer();
		root->add_child_layer(leftmost_panel);
		leftmost_panel->add_child_layer(new editor::CHierarchyPanelLayer());
		leftmost_panel->add_child_layer(new editor::CProjectAssetsPanelLayer());
		if (!m_layerStack.try_push_layer(leftmost_panel))
		{
			if (failed_on.empty()) failed_on = "CLeftmostPanelLayer";
			failed_count++;
			failed = true;
		}
		

		auto viewport = new editor::CViewportPanelLayer();
		root->add_child_layer(viewport);
		if (!m_layerStack.try_push_layer(viewport))
		{
			if (failed_on.empty()) failed_on = "CViewportPanelLayer";
			failed_count++;
			failed = true;
		}


		auto mainmenu = new editor::CMainmenubarLayer();
		root->add_child_layer(mainmenu);
		if (!m_layerStack.try_push_layer(mainmenu))
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


	void CEditor::set_editing_mode()
	{
		auto viewport = get_layer_as< editor::CViewportPanelLayer >("CViewportPanelLayer");
		
		viewport->set_is_enabled(true);
		
		set_engine_fps(0.0f);

		set_engine_window_title("Mint Engine Editor: Editing Mode");

		mint::fx::CCameraManager::Get().set_camera_active("EditorCamera");

		mint::scripting::CBehaviorEngine::Get().set_all_behaviors_active(false);
		mint::scripting::CScriptEngine::Get().set_all_scripts_active(false);
	}


	void CEditor::unset_editing_mode()
	{
		auto viewport = get_layer_as< editor::CViewportPanelLayer >("CViewportPanelLayer");

		viewport->set_is_enabled(false);

		set_engine_fps(30.0f);

		set_engine_window_title("Mint Engine Editor");

		mint::fx::CCameraManager::Get().set_camera_active("DefaultCamera");

		mint::scripting::CBehaviorEngine::Get().set_all_behaviors_active(true);
		mint::scripting::CScriptEngine::Get().set_all_scripts_active(true);
	}






#endif
}