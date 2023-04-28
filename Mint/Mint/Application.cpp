#include "Application.h"


namespace mint
{

#if MINT_DISTR
	bool CApplication::initialize_application(const String& manifest_filepath)
	{
		if(initialize(manifest_filepath))
		{
			IService::print_registered_services();

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

			set_engine_fps(60.0f);
			set_engine_window_title("Mint Engine Editor");

			
			bool result = true;

			
			result &= create_layer_stack();


			return result;
		}

		return false;
	}


	void CEditor::terminate_editor()
	{
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

		
	}


	void CEditor::on_editor_ui_frame_render()
	{
		ui_frame_render();

		if (!m_editingMode) return;


		if (m_showMainMenuBar) show_main_menu_bar();

	}


	void CEditor::on_editor_ui_frame_end()
	{


		ui_frame_end();
	}


	void CEditor::on_editor_before_update()
	{


		on_before_update();
	}


	void CEditor::on_editor_update(f32 dt)
	{
		if (CInput::is_key_pressed_enum(KEY_ESCAPE))
		{
			toggle(m_editingMode);

			if(m_editingMode)
			{
				set_engine_fps(0.0f);

				set_engine_window_title("Mint Engine Editor: Editing Mode");
			}
			else
			{
				set_engine_fps(30.0f);

				set_engine_window_title("Mint Engine Editor");
			}
		}

		on_update(dt);
	}


	void CEditor::on_editor_after_update(f32 dt)
	{


		on_after_update(dt);
	}


	void CEditor::on_editor_frame_begin()
	{
		begin_frame();



	}


	void CEditor::on_editor_frame_render()
	{



		frame();
	}


	void CEditor::on_editor_frame_end()
	{



		end_frame();
	}


	bool CEditor::create_layer_stack()
	{

		return true;
	}


	void CEditor::show_main_menu_bar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Save As..."))
				{
				}
				if (ImGui::MenuItem("Load From..."))
				{
				}
				if (ImGui::MenuItem("Exit"))
				{
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Project"))
			{
				if (ImGui::MenuItem("Decal Database"))
				{
				}

				if (ImGui::MenuItem("Prefab Database"))
				{
				}

				if (ImGui::MenuItem("Rendering Layers"))
				{
				}

				if (ImGui::MenuItem("Entity Database"))
				{
				}

				if (ImGui::MenuItem("Rendering Grid"))
				{
				}

				if (ImGui::MenuItem("Rendering City Territory"))
				{
				}

				if (ImGui::MenuItem("Rendering City Buildind Slots"))
				{
				}

				if (ImGui::MenuItem("Camera Position At Mouse Position"))
				{
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Map"))
			{
				if (ImGui::BeginMenu("Add"))
				{
					if (ImGui::MenuItem("Townhall"))
					{
					}
					if (ImGui::MenuItem("Fort"))
					{
					}
					if (ImGui::MenuItem("Building Slot"))
					{
					}
					if (ImGui::MenuItem("Territory"))
					{
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Audio"))
			{
				ImGui::EndMenu();
			}



			if (ImGui::BeginMenu("Unit"))
			{
				if (ImGui::BeginMenu("Unit 1"))
				{
					if (ImGui::MenuItem("Unit 1.1"))
					{
					}

					if (ImGui::MenuItem("Unit 1.2"))
					{
					}
					
					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Prefab Editor"))
				{
				}


				if (ImGui::MenuItem("Status Effect Editor"))
				{
				}


				if (ImGui::MenuItem("Ability Editor"))
				{
				}


				ImGui::EndMenu();

			}
		}
		ImGui::EndMainMenuBar();
	}


	void CEditor::toggle(bool& value)
	{
		value = !value;
	}


#endif
}