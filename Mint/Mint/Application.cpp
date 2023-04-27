#include "Application.h"


namespace mint
{


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


	bool CEditor::initialize_editor(const String& manifest_filepath)
	{
		if (initialize(manifest_filepath))
		{
			IService::print_registered_services();

			set_engine_fps(60.0f);
			set_engine_window_title("Mint Engine Editor");

			return true;
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


}