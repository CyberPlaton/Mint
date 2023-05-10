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
		TextEditor::Palette palette;

		const auto& light = TextEditor::GetLightPalette();

		for (int i = 0; i < (int)TextEditor::PaletteIndex::Max; i++)
		{
			palette[i] = CUI::get_imgui_color(1.0f, 0.0f, 0.0f, 1.0f);
		}

		// Change identifier, background and function colors.
		palette[(u32)TextEditor::PaletteIndex::Default] = CUI::get_imgui_color(0.9f, 0.9f, 0.9f, 1.0f);							// Light Grey - Good.
		palette[(u32)TextEditor::PaletteIndex::LineNumber] = CUI::get_imgui_color(0.27f, 0.52f, 0.63f, 1.0f);					// Blue - Good.
		palette[(u32)TextEditor::PaletteIndex::Keyword] = CUI::get_imgui_color(0.25f, 0.62f, 0.93f, 1.0f);						// Blue - Good.
		palette[(u32)TextEditor::PaletteIndex::Number] = CUI::get_imgui_color(0.59f, 0.96f, 0.69f, 1.0f);						// Green - Good.
		palette[(u32)TextEditor::PaletteIndex::String] = CUI::get_imgui_color(1.0f, 0.7f, 0.4f, 1.0f);							// Skin Color - Good.
		palette[(u32)TextEditor::PaletteIndex::CharLiteral] = palette[(u32)TextEditor::PaletteIndex::Default];
		palette[(u32)TextEditor::PaletteIndex::Punctuation] = palette[(u32)TextEditor::PaletteIndex::Default];
		palette[(u32)TextEditor::PaletteIndex::Preprocessor] = CUI::get_imgui_color(0.7f, 0.7f, 0.7f, 1.0f);					// Light Grey - Good.
		// Just words and variable names and custom functions := Identifier
		palette[(u32)TextEditor::PaletteIndex::Identifier] = CUI::get_imgui_color(1.0f, 0.84f, 0.62f, 1.0f);					// Light Skin Color - Good.
		// Built-in functions := KnownIdentifier
		palette[(u32)TextEditor::PaletteIndex::KnownIdentifier] = CUI::get_imgui_color(0.86f, 0.46f, 1.0f, 1.0f);				// Violette - Good.
		palette[(u32)TextEditor::PaletteIndex::PreprocIdentifier] = palette[(u32)TextEditor::PaletteIndex::KnownIdentifier];	
		palette[(u32)TextEditor::PaletteIndex::Comment] = CUI::get_imgui_color(0.17f, 0.50f, 0.25f, 1.0f);						// Green - Good.
		palette[(u32)TextEditor::PaletteIndex::MultiLineComment] = palette[(u32)TextEditor::PaletteIndex::Comment];				
		palette[(u32)TextEditor::PaletteIndex::Background] = CUI::get_imgui_color(0.15f, 0.14f, 0.13f, 1.0f);					// Dark Grey - Good.
		palette[(u32)TextEditor::PaletteIndex::Cursor] = CUI::get_imgui_color(1.0f, 1.0f, 1.0f, 1.0f);							// White - Good.
		palette[(u32)TextEditor::PaletteIndex::Selection] = CUI::get_imgui_color(0.67f, 0.79f, 1.0f, 0.4f);						// Light Blue - Good.
		palette[(u32)TextEditor::PaletteIndex::ErrorMarker] = CUI::get_imgui_color(0.65f, 0.02f, 0.0f, 1.0f);					// Dark Red - Good.
		palette[(u32)TextEditor::PaletteIndex::Breakpoint] = palette[(u32)TextEditor::PaletteIndex::ErrorMarker];				
		palette[(u32)TextEditor::PaletteIndex::CurrentLineFill] = CUI::get_imgui_color(0.0f, 0.0f, 0.0f, 0.0f);					// Invisible - Good.
		palette[(u32)TextEditor::PaletteIndex::CurrentLineFillInactive] = palette[(u32)TextEditor::PaletteIndex::CurrentLineFill]; // Invisible - Good.
		palette[(u32)TextEditor::PaletteIndex::CurrentLineEdge] = CUI::get_imgui_color(0.7f, 0.7f, 0.7f, 1.0f);					// Light Grey - Good.

		m_editor.SetPalette(palette);


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


		TextEditor::ErrorMarkers errors;
		errors.emplace(2, "Some weird error");

		m_editor.SetErrorMarkers(errors);
	}


}
#endif