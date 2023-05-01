#include "TextEditor.h"


namespace mint::editor
{


	CTextEditor::CTextEditor(mint::CPath path) :
		m_filepath(path), m_lastSavedFileSize(0), m_currentFileSize(0),
		m_ready(false), m_changed(false), m_windowFlags(0), m_inputFlags(0)
	{
		if (!is_valid_file(path)) return;


		auto buffer = CFileystem::read_file_at_path(path, &m_lastSavedFileSize);

		if (buffer)
		{
			// Copy loaded contents into own buffer.
			std::memset(&m_buffer, NULL, sizeof(m_buffer));
			std::memcpy(&m_buffer, buffer, m_lastSavedFileSize);

			free(buffer);

			set_file_icon(path.get_extension());
			
			m_fileName = String(m_fileIcon) + " " + path.get_stem() + path.get_extension();


			m_lastSavedFileSize = mint::String(m_buffer).size();
			m_currentFileSize = m_lastSavedFileSize;

			m_windowFlags = ImGuiWindowFlags_None;
			m_inputFlags = ImGuiInputTextFlags_None;

			m_ready = true;
		}
	}


	void CTextEditor::on_ui_frame()
	{
		ImGui::Begin(m_fileName.c_str(), NULL, m_windowFlags);

		ImGui::InputTextMultiline("Source", m_buffer, IM_ARRAYSIZE(m_buffer), { std::max(get_current_max_text_width(), s_DefaultEditorTextEditorWidth), s_DefaultEditorTextEditorHeight }, m_inputFlags);
		
		ImGui::SameLine();

		if(ImGui::SmallButton("Save"))
		{
			if(!is_saved()) save_file();
		}

		ImGui::End();

		m_currentFileSize = mint::String(m_buffer).size();
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
		return m_lastSavedFileSize == m_currentFileSize;
	}


	mint::f32 CTextEditor::get_current_max_text_width()
	{
		u32 index = 0;
		u32 max = 0;
		u32 current = 0;
		char character = m_buffer[index];
		while(character != '\0')
		{
			if(character == '\n')
			{
				max = std::max(max, current);

				current = 0;

				index++;
				character = m_buffer[index];

				continue;
			}
			

			current++;
			index++;
			character = m_buffer[index];
		}

		return SCAST(mint::f32, max) + s_DefaultEditorTextEditorWidth;
	}


	void CTextEditor::set_file_icon(const mint::String& file_extension)
	{
		String substr = file_extension.substr(1);

		for(auto i = 0; i < IM_ARRAYSIZE(s_EditorAssetPanelFileTypeExtensions); i++)
		{
			if(strcmp(s_EditorAssetPanelFileTypeExtensions[i], substr.c_str()) == 0)
			{
				m_fileIcon = s_EditorDefaultFileSourceFileIcon[i];
				return;
			}
		}
	}


	bool CTextEditor::save_file()
	{
		void* out_data = malloc(m_currentFileSize);

		std::memcpy(out_data, &m_buffer, m_currentFileSize);

		auto result = CFileReaderWriter::write_to_file_at_path(m_filepath.as_string(), out_data, m_currentFileSize);

		free(out_data);

		return result;
	}


	bool CTextEditor::is_valid_file(mint::CPath path)
	{
		String substr = path.get_extension().substr(1);

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


}