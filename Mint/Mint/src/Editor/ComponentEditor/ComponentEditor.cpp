#if MINT_DISTR
#else
#include "ComponentEditor.h"


namespace mint::editor
{


	CComponentEditor::CComponentEditor(reflection::CMetaClass* metaclass) :
		m_metaclass(metaclass)
	{
		MINT_ASSERT(metaclass != nullptr, "Invalid oepration. Invalid Metaclass provided!");

		m_windowWidth = MINT_ENGINE()->get_main_window_const().get_w();
		m_windowHeight = MINT_ENGINE()->get_main_window_const().get_h();

		m_windowFlags = ImGuiWindowFlags_NoSavedSettings;
		m_active = true;
		m_ready = true;
	}


	void CComponentEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos({ m_windowWidth / 2.0f - w / 2.0f,  m_windowHeight / 2.0f - h / 2.0f }, ImGuiCond_Appearing);

		ImGui::Begin(get_component_name().c_str(), &m_active, m_windowFlags);

		show_members();

		ImGui::End();
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
		return m_metaclass->get_metaclass_name();
	}


	void CComponentEditor::show_members()
	{
		auto slid = GlobalData::Get().s_EditorEditFieldSliderIdStart;
		auto scid = GlobalData::Get().s_EditorEditFieldScalarIdStart;

		for (auto& variant : m_metaclass->get_all_members())
		{
			switch (variant.get_type())
			{
			case reflection::VariantType_Uint32:
			{
				u32* value = variant.cast_safe< u32 >(reflection::VariantType_Uint32);

				u32 __v = *value;

				CUI::edit_field_uint32(__v, 0, 100, variant.get_name(), "", slid, scid, ImGuiSliderFlags_Logarithmic);

				*value = __v;

				break;
			}
			case reflection::VariantType_Uint64:
			{
				u64* value = variant.cast_safe< u64 >(reflection::VariantType_Uint64);

				u64 __v = *value;

				CUI::edit_field_uint64(__v, 0, 100, variant.get_name(), "", slid + 1, scid + 1, ImGuiSliderFlags_Logarithmic);

				*value = __v;


				break;
			}
			case reflection::VariantType_Sint32:
			{
				break;
			}
			case reflection::VariantType_Sint64:
			{
				break;
			}
			case reflection::VariantType_Float:
			{
				break;
			}
			case reflection::VariantType_Double:
			{
				break;
			}
			case reflection::VariantType_String:
			{
				break;
			}
			case reflection::VariantType_Boolean:
			{
				break;
			}
			case reflection::VariantType_Pointer:
			{
				break;
			}
			default:
			{
				break;
			}
			}
		}
	}


}

#endif