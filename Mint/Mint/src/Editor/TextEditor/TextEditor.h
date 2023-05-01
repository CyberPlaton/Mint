#ifndef _MINTEDITOR_TEXT_EDITOR_H_
#define _MINTEDITOR_TEXT_EDITOR_H_


#include "../Common/EditorCommon.h"
#include "Utility/FileSystem/Filesystem.h"


namespace mint::editor
{

	class CTextEditor
	{
	public:
		CTextEditor(mint::CPath path);
		~CTextEditor();

		void on_ui_frame();

		bool is_ready();

		bool is_saved();

		mint::f32 get_current_max_text_width();

		void set_file_icon(const mint::String& file_extension);

	private:
		ImGuiWindowFlags m_windowFlags;
		ImGuiInputTextFlags m_inputFlags;

		bool m_ready;

		bool m_changed;
		mint::u32 m_lastSavedFileSize;
		mint::u32 m_currentFileSize;

		mint::CPath m_filepath;
		mint::String m_fileName;
		mint::String m_fileIcon;

		char m_buffer[s_DefaultEditorTextEditorBufferSize];
	};
}


#endif