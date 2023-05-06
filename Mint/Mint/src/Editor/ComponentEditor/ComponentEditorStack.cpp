#if MINT_DISTR
#else
#include "ComponentEditorStack.h"


namespace mint::editor
{


	bool CComponentEditorStack::try_push_component_editor(reflection::CMetaClass* metaclass)
	{
		if(find_component_editor(metaclass->get_metaclass_name()) >= 0)
		{
			return true;
		}

		auto editor = new CComponentEditor(metaclass);

		if(!editor->is_ready())
		{
			delete editor; editor = nullptr;
			return false;
		}

		m_componentEditors.push_back(editor);

		return true;
	}


	void CComponentEditorStack::pop_component_editor()
	{
		auto editor = mint::algorithm::vector_get_last_element_as< CComponentEditor* >(m_componentEditors);

		mint::algorithm::vector_erase_last(m_componentEditors);

		delete editor; editor = nullptr;
	}


	void CComponentEditorStack::clear_all_component_editors()
	{
		while(!m_componentEditors.empty())
		{
			pop_component_editor();
		}
	}


	void CComponentEditorStack::print_registered_component_editors()
	{
		MINT_LOG_INFO("[{:.4f}][CComponentEditorStack::print_registered_component_editors] Dumping active component editors:", MINT_APP_TIME);

		for(auto& editor: m_componentEditors)
		{
			MINT_LOG_INFO("\tComponent: {}", editor->get_component_name());
		}
	}


	void CComponentEditorStack::on_ui_frame()
	{
		u64 index = 0;
		for(auto& editor: m_componentEditors)
		{
			if(editor->is_active())
			{
				editor->on_ui_frame();
			}
			else
			{
				mint::algorithm::vector_erase_at(m_componentEditors, index);
				break;
			}
			index++;
		}
	}


	CComponentEditorStack::~CComponentEditorStack()
	{
		clear_all_component_editors();
	}


	void CComponentEditorStack::remove_component_editor(const String& component_name)
	{
		s64 index = find_component_editor(component_name);
		if(index >= 0)
		{
			mint::algorithm::vector_erase_at(m_componentEditors, index);
		}
	}


	s64 CComponentEditorStack::find_component_editor(const String& component_name)
	{
		s64 index = 0;
		for (auto & editor : m_componentEditors)
		{
			if (editor->get_component_name() == component_name)
			{
				return index;
			}
			index++;
		}

		return -1;
	}


}

#endif