#if MINT_DISTR
#else
#include "IdentifierComponentEditor.h"

namespace mint::editor
{
	void CIdentifierComponentEditor::on_terminate()
	{

	}


	void CIdentifierComponentEditor::on_update(f32 dt)
	{

	}


	void CIdentifierComponentEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("Identifier", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CIdentifierComponentEditor::show_members()
	{
		auto entity = get_metaclass()->get_metaclass_entity();
		ImGuiID slid = 10000;
		ImGuiID scid = 20000;

		auto name = CUCA::identifier_get_debug_name(entity);
		auto id = CUCA::identifier_get_identifier(entity);
		auto uuid = CUCA::identifier_get_uuid(entity);


		CUI::edit_field_string(name, "Debug Name", "The entity name is for debugging purposes only, it is not unique and irrelevant for the engine internals", slid++, scid++);

		ImGui::Text(TextFormat("ID: %zu", id)); CUI::help_marker_no_question_mark("The entity ID is the application unique way to find this specific entity. Read-only, as the value is relevant to engine internals");

		ImGui::Text(TextFormat("UUID: %zu", uuid)); CUI::help_marker_no_question_mark("The entity UUID is the universally unique way to find this specific entity. Read-only, as the value is relevant to engine internals");
	
		CUCA::identifier_set_debug_name(entity, name);
	}


	CIdentifierComponentEditor::CIdentifierComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif