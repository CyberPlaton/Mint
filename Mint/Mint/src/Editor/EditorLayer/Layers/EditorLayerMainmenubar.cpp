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
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					MINT_ENGINE()->exit();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Project"))
			{
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