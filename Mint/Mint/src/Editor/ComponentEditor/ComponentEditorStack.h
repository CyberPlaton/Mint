#if MINT_DISTR
#else
#ifndef _MINTEDITOR_COMPONENT_EDITOR_STACK_H_
#define _MINTEDITOR_COMPONENT_EDITOR_STACK_H_


#include "ComponentEditor.h"


namespace mint::editor
{

	class CComponentEditorStack
	{
	public:
		CComponentEditorStack() = default;
		~CComponentEditorStack();

		bool try_push_component_editor(reflection::CMetaClass* metaclass);

		void pop_component_editor();

		void remove_component_editor(const String& component_name);

		void clear_all_component_editors();

		void print_registered_component_editors();

		s64 find_component_editor(const String& component_name);

		void on_ui_frame();


	private:
		Vector< CComponentEditor* > m_componentEditors;

	};

}

#endif
#endif