#if MINT_DISTR
#else
#ifndef _MINTEDITOR_TEXT_EDITOR_H_
#define _MINTEDITOR_TEXT_EDITOR_H_


#include "../Common/EditorCommon.h"
#include "Utility/FileSystem/Filesystem.h"
#include "ImGuiTextEditor.h"



namespace mint::editor
{
	class CTextEditor
	{
	public:
		CTextEditor(mint::CPath path);
		~CTextEditor();

		bool is_valid_file(mint::CPath path);

		void on_ui_frame();

		bool is_ready();

		bool is_active();

		bool is_saved();

		bool save_file();

		void set_file_icon(const mint::String& file_extension);

		void set_error(u64 line, const mint::String& error_message);

		void check_for_errors();

	private:
		ImGuiWindowFlags m_windowFlags;
		ImGuiInputTextFlags m_inputFlags;

		bool m_active;

		bool m_ready;

		bool m_changed;
		bool m_saved;

		mint::CPath m_filepath;
		mint::String m_fileName;
		mint::String m_fileType;
		mint::String m_fileIcon;

		TextEditor m_editor;

		TextEditor::ErrorMarkers m_errors;

	private:
		void _prepare_editor(const String& file_type);
	};
}


#endif
#endif