#if MINT_DISTR
#else
#include "TextEditor.h"


namespace mint::editor
{


	CTextEditor::CTextEditor(mint::CPath path) :
		m_filepath(path), m_ready(false), m_changed(false), m_saved(false), m_windowFlags(0), m_inputFlags(0)
	{
		if (!is_valid_file(path)) return;


		u32 file_size = 0;
		auto buffer = CFileystem::read_file_at_path(path, &file_size);

		if (buffer)
		{
			m_editor.SetText(buffer);

			free(buffer);

			set_file_icon(path.get_extension());

			m_fileName = String(m_fileIcon) + " " + path.get_stem() + path.get_extension();

			m_windowFlags = ImGuiWindowFlags_None;
			m_inputFlags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine;

			m_ready = true;
			m_active = true;
			m_saved = true;

			_prepare_editor(path.get_extension());
		}
	}


	void CTextEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorTextEditorWidth;
		auto h = GlobalData::Get().s_DefaultEditorTextEditorHeight;

		String input_text = "##" + m_fileName;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::Begin(m_fileName.c_str(), &m_active, m_windowFlags);

		auto current_w = ImGui::GetWindowWidth();
		auto current_h = ImGui::GetWindowHeight();

		w = glm::max(w, current_w);
		h = glm::max(h, current_h);

		ImGui::SameLine();
		ImGui::Text(is_saved() == true ? ICON_FA_CHECK : ICON_FA_STAR_OF_LIFE);

		ImGui::SameLine();
		if (ImGui::SmallButton("Save File"))
		{
			if (!is_saved()) save_file();
		}

		m_editor.Render(input_text.c_str(), { w - 10.0f, h - 10.0f }, true);

 		ImGui::End();

		m_changed = m_editor.IsTextChanged();


		if(m_changed)
		{
			m_saved = false;
		}
	}


	bool CTextEditor::is_ready()
	{
		return m_ready;
	}


	CTextEditor::~CTextEditor()
	{
	}


	bool CTextEditor::is_saved()
	{
		return m_saved;
	}



	void CTextEditor::set_file_icon(const mint::String& file_extension)
	{
		String substr = file_extension;

		for (auto i = 0; i < IM_ARRAYSIZE(s_EditorAssetPanelFileTypeExtensions); i++)
		{
			if (strcmp(s_EditorAssetPanelFileTypeExtensions[i], substr.c_str()) == 0)
			{
				m_fileIcon = s_EditorDefaultFileSourceFileIcon[i];
				return;
			}
		}
	}


	bool CTextEditor::save_file()
	{
		const auto& text = m_editor.GetText();

		if(!CFileReaderWriter::write_to_file_at_path(m_filepath.as_string(), (void*)text.c_str(), text.size()))
		{
			MINT_LOG_ERROR("[{:.4f}][CTextEditor::save_file] Failed saving file to \"{}\"!", MINT_APP_TIME, m_filepath.as_string());

			return false;
		}

		m_saved = true;
		return true;
	}


	bool CTextEditor::is_valid_file(mint::CPath path)
	{
		String substr = path.get_extension();

		for (auto i = 0; i < IM_ARRAYSIZE(s_EditorAssetPanelFileTypeExtensions); i++)
		{
			if (strcmp(s_EditorAssetPanelFileTypeExtensions[i], substr.c_str()) == 0)
			{
				return true;
			}
		}

		MINT_LOG_WARN("[{:.4f}][CTextEditor::is_valid_file] Unable to open file \"{}\" with invalid type: \"{}\"!", MINT_APP_TIME, path.as_string(), substr);

		return false;
	}


	bool CTextEditor::is_active()
	{
		return m_active;
	}


	void CTextEditor::_prepare_editor(const String& file_type)
	{
		m_editor.SetPalette(TextEditor::GetLightPalette());

		if(file_type == ".lua")
		{
			m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

			MINT_LOG_WARN("[{:.4f}][CTextEditor::_prepare_editor] Syntax highlighting for Scripting is not yet fully supported!", MINT_APP_TIME);
		}
		else if(file_type == ".fsh" || file_type == ".vsh")
		{
			m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
		}
		else if (file_type == ".maml" || file_type == ".scene" || file_type == ".texture" ||
				 file_type == ".behavior" || file_type == ".script" || file_type == ".shader")
		{
			m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::MAML());

			MINT_LOG_WARN("[{:.4f}][CTextEditor::_prepare_editor] Syntax highlighting for MAML is not yet fully supported!", MINT_APP_TIME);
		}
		else
		{
			MINT_LOG_ERROR("[{:.4f}][CTextEditor::_prepare_editor] Syntax highlighting for Type \"{}\" is not supported!", MINT_APP_TIME, file_type);
		}

	}


}
#endif