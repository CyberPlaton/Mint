#if MINT_DISTR
#else
#include "ComponentEditorStack.h"


namespace mint::editor
{


	bool CComponentEditorStack::try_push_component_editor(reflection::CMetaClass* metaclass)
	{
		String metaclass_name = metaclass->get_metaclass_name();

		if(find_component_editor(metaclass_name) >= 0)
		{
			return true;
		}

		IComponentEditor* editor = nullptr;

		if(metaclass_name == "SIdentifier")
		{
			editor = new CIdentifierComponentEditor(metaclass);
		}
		else if (metaclass_name == "SSceneHierarchy")
		{
			editor = new CSceneHierarchyComponentEditor(metaclass);
		}
		else if (metaclass_name == "SRigidBody")
		{
			editor = new CRigidBodyComponentEditor(metaclass);
		}
		else if (metaclass_name == "STransform")
		{
			editor = new CTransformComponentEditor(metaclass);
		}
		else if (metaclass_name == "SSprite")
		{
			editor = new CSpriteComponentEditor(metaclass);
		}
		else if (metaclass_name == "SAnimatedSprite")
		{
			editor = new CAnimatedSpriteComponentEditor(metaclass);
		}
		else if (metaclass_name == "SScript")
		{
			editor = new CScriptComponentEditor(metaclass);
		}
		else
		{
			editor = new CGeneralComponentEditor(metaclass);
		}

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
		auto editor = mint::algorithm::vector_get_last_element_as< IComponentEditor* >(m_componentEditors);

		editor->on_terminate();

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
		for (auto& editor : m_componentEditors)
		{
			if (editor->is_active())
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


	void CComponentEditorStack::on_update(f32 dt)
	{
		u64 index = 0;
		for (auto& editor : m_componentEditors)
		{
			if (editor->is_active())
			{
				editor->on_update(dt);
			}
			else
			{
				mint::algorithm::vector_erase_at(m_componentEditors, index);
				break;
			}
			index++;
		}
	}


}

#endif