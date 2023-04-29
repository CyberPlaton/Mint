#include "EditorLayerMainmenubar.h"


namespace mint::editor
{


	bool CMainmenubarLayer::on_initialize()
	{
		return true;
	}


	void CMainmenubarLayer::on_update(f32 dt)
	{

	}


	void CMainmenubarLayer::on_ui_frame()
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


	mint::String CMainmenubarLayer::get_layer_name()
	{
		return "Main Menu Bar";
	}


}