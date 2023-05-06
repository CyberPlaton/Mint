#if MINT_DISTR
#else
#ifndef _MINTEDITOR_TEXT_EDITOR_H_
#define _MINTEDITOR_TEXT_EDITOR_H_


#include "../Common/EditorCommon.h"
#include "Utility/FileSystem/Filesystem.h"


namespace mint::editor
{
#ifndef MINTEDITOR_TEXT_EDITOR_BUFFER_SIZE
#define MINTEDITOR_TEXT_EDITOR_BUFFER_SIZE 1024 * 13
#endif

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

	private:
		ImGuiWindowFlags m_windowFlags;
		ImGuiInputTextFlags m_inputFlags;

		bool m_active;

		bool m_ready;

		bool m_changed;
		mint::u32 m_lastSavedFileSize;
		mint::u32 m_currentFileSize;

		mint::CPath m_filepath;
		mint::String m_fileName;
		mint::String m_fileIcon;

		char m_buffer[MINTEDITOR_TEXT_EDITOR_BUFFER_SIZE];
	};
}


#endif
#endif