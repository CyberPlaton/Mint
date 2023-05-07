#if MINT_DISTR
#else
#include "GeneralComponentEditor.h"


namespace mint::editor
{


	CGeneralComponentEditor::CGeneralComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


	void CGeneralComponentEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin(get_component_name().c_str(), &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CGeneralComponentEditor::show_members()
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
			case reflection::VariantType_Vec2:
			{
				break;
			}
			case reflection::VariantType_Vec3:
			{
				break;
			}
			case reflection::VariantType_Vec4:
			{
				break;
			}
			case reflection::VariantType_Rect:
			{
				break;
			}
			case reflection::VariantType_Color:
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


	void CGeneralComponentEditor::on_terminate()
	{

	}


	void CGeneralComponentEditor::on_update(f32 dt)
	{

	}


}

#endif