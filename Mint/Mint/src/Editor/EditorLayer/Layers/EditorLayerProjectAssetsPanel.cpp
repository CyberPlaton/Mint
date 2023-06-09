#if MINT_DISTR
#else
#include "EditorLayerProjectAssetsPanel.h"


namespace mint::editor
{


	bool CProjectAssetsPanelLayer::on_initialize()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::on_initialize");
		return true;
	}


	void CProjectAssetsPanelLayer::on_update(f32 dt)
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::on_update");
		mint::CFilesystem fs(mint::CFilesystem::get_working_directory());

		mint::String scene_name = MINT_ACTIVE_SCENE()->get_scene_name();

		if (fs.forward("Scenes") && fs.forward_pretend(scene_name))
		{
			m_currentScenePathFull = mint::CFilesystem::construct_from(fs.get_current_directory().as_string(), scene_name).as_string();
			m_currentScenePathRelative = mint::CFilesystem::construct_from("Scenes", scene_name).as_string();
			m_currentSceneName = scene_name;
		}
	}


	void CProjectAssetsPanelLayer::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::on_ui_frame");
		auto width = ImGui::GetWindowWidth();
		auto height = ImGui::GetWindowHeight();


		ImGui::BeginChild("Project Assets", { width - 17.5f, percent(height, 48) }, true, get_flags());

		menu_bar();

		main_frame();

		ImGui::EndChild();

		if (m_createFolderDialog) show_create_folder_dialog();
		if (m_createFileDialog) show_create_file_dialog();
		if (m_removeDialog) show_delete_folder_dialog();


		for (auto i = 0; i < m_textEditorStack.size(); i++)
		{
			auto& texteditor = m_textEditorStack[i];

			texteditor.on_ui_frame();

			if (!texteditor.is_active())
			{
				m_textEditorStack.erase(m_textEditorStack.begin() + i);
				break;
			}

		}
	}


	mint::String CProjectAssetsPanelLayer::get_layer_name()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::get_layer_name");
		return "CProjectAssetsPanelLayer";
	}


	ImGuiWindowFlags CProjectAssetsPanelLayer::get_flags()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::get_flags");
		return ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


	void CProjectAssetsPanelLayer::menu_bar()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::menu_bar");
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
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::main_frame");
		CFilesystem fs(m_currentScenePathFull);

		// Create "assets" folder if not already there.
		if (!fs.forward_pretend(GlobalData::Get().s_EditorDefaultSceneRessourcesPath))
		{
			CFilesystem::create_directory(CFilesystem::construct_from(fs.get_current_directory().as_string(), GlobalData::Get().s_EditorDefaultSceneRessourcesPath));
		}

		const bool open = ImGui::TreeNode(m_currentSceneName.c_str());

		CUI::help_marker_no_question_mark(m_currentScenePathFull.c_str());

		if (open)
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
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::show_folder_contents");
		String text = ICON_FA_FOLDER_TREE + path.get_stem();

		if (ImGui::TreeNode(text.c_str()))
		{
			show_folder_options(path);

			CFilesystem fs(path);

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
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::show_file");
		// Show this icon if it is editable, otherwise just as plain text.
		mint::String text;
		bool editable = true;
		if(is_file_editable(path.get_extension()))
		{
			text = mint::String(ICON_FA_PEN_TO_SQUARE) + " " + path.get_stem() + path.get_extension();
		}
		else
		{
			editable = false;
			text = path.get_stem() + path.get_extension();
		}

		
		if(editable)
		{
			if (ImGui::SmallButton(text.c_str()))
			{
				CPath relative = CFilesystem::get_relative_path_to_working_directory(path);

				m_textEditorStack.emplace_back(relative);

				auto& texteditor = mint::algorithm::vector_get_last_element_as<CTextEditor&>(m_textEditorStack);

				if (!texteditor.is_ready())
				{
					mint::algorithm::vector_erase_last(m_textEditorStack);
				}
			}
		}
		else
		{
			ImGui::Text(text.c_str());
		}
	}


	void CProjectAssetsPanelLayer::show_folder_options(CPath& path)
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::show_folder_options");
		String text = "##" + path.as_string();

		static int item_current = 0;
		ImGui::SameLine();

		ImGui::SetNextItemWidth(GlobalData::Get().s_DefaultComboWidth);
		if (ImGui::Combo(text.c_str(), &item_current, s_EditorAssetPanelFolderOptions, IM_ARRAYSIZE(s_EditorAssetPanelFolderOptions)))
		{
			switch (item_current)
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
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::show_file_options");
	}


	void CProjectAssetsPanelLayer::show_create_folder_dialog()
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::show_create_folder_dialog");
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;


		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f, get_window_height() / 2.0f - h / 2.0f });
		ImGui::SetNextWindowSize({ w, h });

		String text = "New folder at " + m_createDirectory.get_stem();
		String itext = "##" + text;

		ImGui::Begin(text.c_str(), &m_createFolderDialog);

		ImGui::InputText(itext.c_str(), m_createDialogBuffer, sizeof(m_createDialogBuffer), ImGuiInputTextFlags_None);


		if (ImGui::SmallButton("OK"))
		{
			mint::String name = mint::String(m_createDialogBuffer);

			if (!name.empty())
			{
				// create folder
				auto path = CFilesystem::construct_from(m_createDirectory, name);

				if (CFilesystem::create_directory(path))
				{
				}
			}

			m_createFolderDialog = false;
			m_createDirectory = CPath();
			std::memset(&m_createDialogBuffer, NULL, sizeof(m_createDialogBuffer));
		}
		ImGui::SameLine();
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
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::show_create_file_dialog");
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w, get_window_height() / 2.0f - h });
		ImGui::SetNextWindowSize({ w, h });

		String text = "New file at " + m_createDirectory.get_stem();

		ImGui::Begin(text.c_str(), &m_createFileDialog);

		ImGui::InputText("## |", m_createDialogBuffer, sizeof(m_createDialogBuffer), ImGuiInputTextFlags_None);

		static int item_current = 0;
		String extension;
		ImGui::SameLine();

		ImGui::SetNextItemWidth(GlobalData::Get().s_DefaultComboWidth);
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
				if (CFilesystem::create_file(m_createDirectory, name, extension, false))
				{
				}
			}

			m_createFileDialog = false;
			m_createDirectory = CPath();
			std::memset(&m_createDialogBuffer, NULL, sizeof(m_createDialogBuffer));
		}
		ImGui::SameLine();
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
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::show_delete_folder_dialog");
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;


		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w, get_window_height() / 2.0f - h });
		ImGui::SetNextWindowSize({ w, h });

		String text = "Delete " + m_removeDirectory.get_stem() + " with all contents?";

		ImGui::Begin(text.c_str(), &m_removeDialog);

		if (ImGui::SmallButton("OK"))
		{
			if (CFilesystem::delete_directory_or_file(m_removeDirectory))
			{
			}

			m_removeDialog = false;
			m_removeDirectory = CPath();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Cancel"))
		{
			m_removeDialog = false;
			m_removeDirectory = CPath();
		}

		ImGui::End();
	}


	bool CProjectAssetsPanelLayer::is_file_editable(const String& file_extension)
	{
		MINT_PROFILE_SCOPE("Editor::Layer", "CProjectAssetsPanelLayer::is_file_editable");
		for(const auto& ext: s_EditorAssetPanelFileTypeExtensions)
		{
			if (strcmp(file_extension.c_str(), ext) == 0) return true;
		}

		return false;
	}


}
#endif