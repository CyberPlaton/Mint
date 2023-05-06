#if MINT_DISTR
#else
#include "ComponentEditor.h"


namespace mint::editor
{


	CComponentEditor::CComponentEditor(reflection::CMetaClass* metaclass) : 
		IComponentEditor(), m_metaclass(metaclass)
	{
		MINT_ASSERT(metaclass != nullptr, "Invalid oepration. Invalid Metaclass provided!");

		m_windowWidth = MINT_ENGINE()->get_main_window_const().get_w();
		m_windowHeight = MINT_ENGINE()->get_main_window_const().get_h();

		set_flags(ImGuiWindowFlags_NoSavedSettings);
		
		set_component_name(metaclass->get_metaclass_name());

		set_ready(false);
		set_active(false);
	}


	CComponentEditor::~CComponentEditor()
	{
		m_metaclass = nullptr;
		m_windowFlags = 0;
	}


	void CComponentEditor::set_ready(bool value)
	{
		m_ready = value;
	}


	void CComponentEditor::set_active(bool value)
	{
		m_active = value;
	}


	void CComponentEditor::set_component_name(const String& value)
	{
		m_componentName = value;
	}


	mint::f32 CComponentEditor::get_window_width()
	{
		return m_windowWidth;
	}


	mint::f32 CComponentEditor::get_window_height()
	{
		return m_windowHeight;
	}


	ImGuiWindowFlags CComponentEditor::get_flags()
	{
		return m_windowFlags;
	}


	void CComponentEditor::set_flags(ImGuiWindowFlags value)
	{
		m_windowFlags = value;
	}


	bool CComponentEditor::is_ready()
	{
		return m_ready;
	}


	bool CComponentEditor::is_active()
	{
		return m_active;
	}


	mint::String CComponentEditor::get_component_name()
	{
		return m_componentName;
	}


}

#endif