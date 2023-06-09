#if MINT_DISTR
#else
#include "EditorLayerMainmenubar.h"


namespace mint::editor
{


	bool CMainmenubarLayer::on_initialize()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CMainmenubarLayer::on_initialize");
		return true;
	}


	void CMainmenubarLayer::on_update(f32 dt)
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CMainmenubarLayer::on_update");
	}


	void CMainmenubarLayer::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CMainmenubarLayer::on_ui_frame");
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::BeginMenu("Language"))
				{
					if (ImGui::MenuItem("English"))
					{
						set_used_language(lang::Language_English);
					}
					if (ImGui::MenuItem("German"))
					{
						set_used_language(lang::Language_German);
					}
					if (ImGui::MenuItem("Russian"))
					{
						set_used_language(lang::Language_Russian);
					}
					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Exit"))
				{
					MINT_ENGINE()->exit();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("File"))
			{
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
			if (ImGui::BeginMenu("Debug"))
			{
				ImGui::Checkbox("Profiler", &editor::GlobalData::Get().s_EditorProfilerEnabled);

				ImGui::EndMenu();
			}

		}
		ImGui::EndMainMenuBar();
	}


	mint::String CMainmenubarLayer::get_layer_name()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CMainmenubarLayer::get_layer_name");
		return "CMainmenubarLayer";
	}


}
#endif