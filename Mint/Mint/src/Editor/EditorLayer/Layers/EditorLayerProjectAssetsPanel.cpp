#if MINT_DISTR
#else
#include "EditorLayerProjectAssetsPanel.h"


namespace mint::editor
{


	bool CProjectAssetsPanelLayer::on_initialize()
	{
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
		return "CProjectAssetsPanelLayer";
	}


	ImGuiWindowFlags CProjectAssetsPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


	void CProjectAssetsPanelLayer::menu_bar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Project Assets"))
			{
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}


	void CProjectAssetsPanelLayer::main_frame()
	{
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
		String content_name = path.get_stem();

		const bool open = ImGui::TreeNode(content_name.c_str());

		ImGui::SameLine();

		if (open) ImGui::Text(ICON_FA_FOLDER_OPEN);
		else ImGui::Text(ICON_FA_FOLDER);

		if (open)
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
		static s32 selected_option_item = -1;
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
		if (ImGui::SmallButton(ICON_FA_GEAR))
		{
			ImGui::OpenPopup("Folder_Options_Popup");
		}
		ImGui::PopStyleColor();


		if (ImGui::BeginPopup("Folder_Options_Popup"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(s_EditorAssetPanelFolderOptions); i++)
			{
				if (ImGui::Selectable(s_EditorAssetPanelFolderOptions[i])) { selected_option_item = i; }
			}

			ImGui::EndPopup();
		}

		if (selected_option_item >= 0)
		{
			switch ((FolderOptions)selected_option_item)
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

			selected_option_item = -1;
		}
	}


	void CProjectAssetsPanelLayer::show_file_options(CPath& path)
	{
	}


	void CProjectAssetsPanelLayer::show_create_folder_dialog()
	{
		String text = "New folder at \"" + m_createDirectory.get_stem() + "\"";

		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;
		Vec2 position = { get_window_width() / 2.0f - w, get_window_height() / 2.0f - h }, size = { w, h };

		CUI::create_folder_dialog(text, "", &m_createFolderDialog, position, size, m_createDirectory.as_string());

		if(!m_createFolderDialog) m_createDirectory = CPath();
	}


	void CProjectAssetsPanelLayer::show_create_file_dialog()
	{
		String text = "New file at \"" + m_createDirectory.get_stem() + "\"";

		auto w = GlobalData::Get().s_DefaultEditorDialogWidth + 125.0f;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;
		Vec2 position = { get_window_width() / 2.0f - w, get_window_height() / 2.0f - h }, size = { w, h };

		CUI::create_file_dialog(text, "", &m_createFileDialog, position, size, s_EditorAssetPanelFileTypeExtensions, IM_ARRAYSIZE(s_EditorAssetPanelFileTypeExtensions), m_createDirectory.as_string());

		if(!m_createFileDialog) m_createDirectory = CPath();
	}



	void CProjectAssetsPanelLayer::show_delete_folder_dialog()
	{
		String text = "Delete folder \"" + m_removeDirectory.get_stem() + "\" with all contents?";

		auto w = GlobalData::Get().s_DefaultEditorDialogWidth + 125.0f;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;
		Vec2 position = { get_window_width() / 2.0f - w, get_window_height() / 2.0f - h }, size = { w, h };

		CUI::delete_folder_dialog(text, "", &m_removeDialog, position, size, m_removeDirectory.as_string());

		if (!m_removeDialog) m_removeDirectory = CPath();
	}


	bool CProjectAssetsPanelLayer::is_file_editable(const String& file_extension)
	{
		for(const auto& ext: s_EditorAssetPanelFileTypeExtensions)
		{
			if (strcmp(file_extension.c_str(), ext) == 0) return true;
		}

		return false;
	}


}
#endif