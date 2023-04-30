#include "EditorLayerProjectAssetsPanel.h"


namespace mint::editor
{


	bool CProjectAssetsPanelLayer::on_initialize()
	{
		return true;
	}


	void CProjectAssetsPanelLayer::on_update(f32 dt)
	{
		mint::CFileystem fs(mint::CFileystem::get_working_directory());

		mint::String scene_name = MINT_ACTIVE_SCENE()->get_scene_name();

		if (fs.forward("Scenes") && fs.forward_pretend(scene_name))
		{
			m_currentScenePathFull = mint::CFileystem::construct_from(fs.get_current_directory().as_string(), scene_name).as_string();
			m_currentScenePathRelative = mint::CFileystem::construct_from("Scenes", scene_name).as_string();
			m_currentSceneName = scene_name;
		}
	}


	void CProjectAssetsPanelLayer::on_ui_frame()
	{
		auto width = ImGui::GetWindowWidth();
		auto height = ImGui::GetWindowHeight();

		
		ImGui::BeginChild("Project Assets", { width, percent(height, 30) }, true, get_flags());

		menu_bar();

		main_frame();

		ImGui::EndChild();
	}


	mint::String CProjectAssetsPanelLayer::get_layer_name()
	{
		return "Project Assets Panel";
	}


	ImGuiWindowFlags CProjectAssetsPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


	void CProjectAssetsPanelLayer::menu_bar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu(u8"Project Assets"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"Project Assets"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"Project Assets"))
			{
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}


	void CProjectAssetsPanelLayer::main_frame()
	{
// 		CUI::image_button(CEditorIconManager::Get().get_texture_imgui("plus"), { 32, 32 });
// 		ImGui::SameLine();
// 		CUI::help_marker("Add something or something");

		CFileystem fs(m_currentScenePathFull);

		// Create "assets" folder if not already there.
		if(!fs.forward_pretend(editor::s_EditorDefaultSceneRessourcesPath))
		{
			CFileystem::create_directory(CFileystem::construct_from(fs.get_current_directory().as_string(), editor::s_EditorDefaultSceneRessourcesPath));
		}

		if(ImGui::TreeNode(m_currentSceneName.c_str()))
		{
			auto dirs = fs.get_all_directories_in_current_dir();
			auto files = fs.get_all_files_in_current_dir();

			for (auto& dir : dirs)
			{
				show_folder_contents(dir);
			}
			for (auto& file : files)
			{
				show_file(file.get_stem() + "." + file.get_extension());
			}

			ImGui::TreePop();
		}
	}


	void CProjectAssetsPanelLayer::show_folder_contents(CPath& path)
	{
		if(ImGui::TreeNode(path.get_stem().c_str()))
		{
			CFileystem fs(path);

			auto dirs = fs.get_all_directories_in_current_dir();
			auto files = fs.get_all_files_in_current_dir();

			for (auto& dir : dirs)
			{
				show_folder_contents(dir);
			}
			for (auto& file : files)
			{
				show_file(file.get_stem() + "." + file.get_extension());
			}



			ImGui::TreePop();
		}
	}


	void CProjectAssetsPanelLayer::show_file(const String& file_path)
	{
		ImGui::Text(file_path.c_str());
	}


}