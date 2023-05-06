#if MINT_DISTR
#else
#ifndef _MINTEDITOR_COMPONENT_EDITOR_H_
#define _MINTEDITOR_COMPONENT_EDITOR_H_


#include "../Common/EditorCommon.h"
#include "Components/Components.h"
#include "../../IMint.h"


namespace mint::editor
{

	class CComponentEditor
	{
	public:
		CComponentEditor(reflection::CMetaClass* metaclass);

		void on_ui_frame();

		bool is_ready();

		bool is_active();

		String get_component_name();

	private:
		reflection::CMetaClass* m_metaclass;

		ImGuiWindowFlags m_windowFlags;

		bool m_active;

		bool m_ready;

		f32 m_windowWidth;
		f32 m_windowHeight;

	private:
		void show_members();
	};

}

#endif
#endif