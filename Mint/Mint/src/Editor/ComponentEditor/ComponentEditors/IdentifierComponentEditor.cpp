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

	}


	CIdentifierComponentEditor::CIdentifierComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif