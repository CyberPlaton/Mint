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

		if (m_createFolderDialog) show_create_folder_dialog();
		if (m_createFileDialog) show_create_file_dialog();
		if (m_removeDialog) show_delete_folder_dialog();
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
		CFileystem fs(m_currentScenePathFull);

		// Create "assets" folder if not already there.
		if(!fs.forward_pretend(editor::s_EditorDefaultSceneRessourcesPath))
		{
			CFileystem::create_directory(CFileystem::construct_from(fs.get_current_directory().as_string(), editor::s_EditorDefaultSceneRessourcesPath));
		}

		const bool open = ImGui::TreeNode(m_currentSceneName.c_str());

		CUI::help_marker_no_question_mark(m_currentScenePathFull.c_str());

		if(open)
		{
			auto dirs = fs.get_all_directories_in_current_dir();
			auto files = fs.get_all_files_in_current_dir();

			for (auto& dir : dirs)
			{
				show_folder_contents(dir);
			}
			for (auto& file : files)
			{
				show_file(file);
			}

			ImGui::TreePop();
		}
	}


	void CProjectAssetsPanelLayer::show_folder_contents(CPath& path)
	{
		if(ImGui::TreeNode(path.get_stem().c_str()))
		{
			show_folder_options(path);

			CFileystem fs(path);

			auto dirs = fs.get_all_directories_in_current_dir();
			auto files = fs.get_all_files_in_current_dir();

			for (auto& dir : dirs)
			{
				show_folder_contents(dir);
			}
			for (auto& file : files)
			{
				show_file(file);
			}

			ImGui::TreePop();
		}
	}


	void CProjectAssetsPanelLayer::show_file(CPath& path)
	{
		mint::String file = path.get_stem() + path.get_extension();
		ImGui::Text(file.c_str());
	}


	void CProjectAssetsPanelLayer::show_folder_options(CPath& path)
	{
		static int item_current = 0;
		ImGui::SameLine();

		ImGui::SetNextItemWidth(25.0f);
		if(ImGui::Combo("|", &item_current, s_EditorAssetPanelFolderOptions, IM_ARRAYSIZE(s_EditorAssetPanelFolderOptions)))
		{
			switch(item_current)
			{
			case FolderOptions_NewFolder:
			{
				m_createFolderDialog = true;
				m_createDirectory = path;
				break;
			}
			case FolderOptions_NewFile:
			{
				m_createFileDialog = true;
				m_createDirectory = path;
				break;
			}
			case FolderOptions_Delete:
			{
				m_removeDialog = true;
				m_removeDirectory = path;
				break;
			}
			default: break;
			}
		}
	}


	void CProjectAssetsPanelLayer::show_file_options(CPath& path)
	{
		
	}


	void CProjectAssetsPanelLayer::show_create_folder_dialog()
	{
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - s_DefaultEditorDialogWidth, get_window_height() / 2.0f - s_DefaultEditorDialogHeight });
		ImGui::SetNextWindowSize({ s_DefaultEditorDialogWidth, s_DefaultEditorDialogHeight });

		String text = "New folder at " + m_createDirectory.get_stem();

		ImGui::Begin(text.c_str(), &m_createFolderDialog);

		ImGui::InputText("|", m_createDialogBuffer, sizeof(m_createDialogBuffer), ImGuiInputTextFlags_None);


		if(ImGui::SmallButton("OK"))
		{
			mint::String name = mint::String(m_createDialogBuffer);

			if(!name.empty())
			{
				// create folder
				auto path = CFileystem::construct_from(m_createDirectory, name);
				
				if(CFileystem::create_directory(path))
				{
				}
			}

			m_createFolderDialog = false;
			m_createDirectory = CPath();
			std::memset(&m_createDialogBuffer, NULL, sizeof(m_createDialogBuffer));
		}
		if (ImGui::SmallButton("Cancel"))
		{
			m_createFolderDialog = false;
			m_createDirectory = CPath();
			std::memset(&m_createDialogBuffer, NULL, sizeof(m_createDialogBuffer));
		}


		ImGui::End();
	}


	void CProjectAssetsPanelLayer::show_create_file_dialog()
	{
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - s_DefaultEditorDialogWidth, get_window_height() / 2.0f - s_DefaultEditorDialogHeight });
		ImGui::SetNextWindowSize({ s_DefaultEditorDialogWidth, s_DefaultEditorDialogHeight });

		String text = "New file at " + m_createDirectory.get_stem();

		ImGui::Begin(text.c_str(), &m_createFileDialog);

		ImGui::InputText("|", m_createDialogBuffer, sizeof(m_createDialogBuffer), ImGuiInputTextFlags_None);

		static int item_current = 0;
		String extension;
		ImGui::SameLine();

		ImGui::SetNextItemWidth(25.0f);
		ImGui::Combo(s_EditorAssetPanelFileTypes[item_current], &item_current, s_EditorAssetPanelFileTypes, IM_ARRAYSIZE(s_EditorAssetPanelFileTypes));
		
		ImGui::SameLine();
		CUI::help_marker("Select the Type of the File to be created!");

		if (ImGui::SmallButton("OK"))
		{
			mint::String name = mint::String(m_createDialogBuffer);

			if (!name.empty())
			{
				extension = String(s_EditorAssetPanelFileTypeExtensions[item_current]);

				// create file
				if(CFileystem::create_file(m_createDirectory, name, extension))
				{
				}
			}

			m_createFileDialog = false;
			m_createDirectory = CPath();
			std::memset(&m_createDialogBuffer, NULL, sizeof(m_createDialogBuffer));
		}
		if (ImGui::SmallButton("Cancel"))
		{
			m_createFileDialog = false;
			m_createDirectory = CPath();
			std::memset(&m_createDialogBuffer, NULL, sizeof(m_createDialogBuffer));
		}


		ImGui::End();
	}



	void CProjectAssetsPanelLayer::show_delete_folder_dialog()
	{
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - s_DefaultEditorDialogWidth, get_window_height() / 2.0f - s_DefaultEditorDialogHeight });
		ImGui::SetNextWindowSize({ s_DefaultEditorDialogWidth, s_DefaultEditorDialogHeight });

		String text = "Delete " + m_removeDirectory.get_stem() + " with all contents?";

		ImGui::Begin(text.c_str(), &m_removeDialog);

		if (ImGui::SmallButton("OK"))
		{
			if(CFileystem::delete_directory_or_file(m_removeDirectory))
			{
			}

			m_removeDialog = false;
			m_removeDirectory = CPath();
		}
		if (ImGui::SmallButton("Cancel"))
		{
			m_removeDialog = false;
			m_removeDirectory = CPath();
		}

		ImGui::End();
	}

}