#if MINT_DISTR
#else
#include "ScriptComponentEditor.h"

namespace mint::editor
{
	void CScriptComponentEditor::on_terminate()
	{

	}


	void CScriptComponentEditor::on_update(f32 dt)
	{

	}


	void CScriptComponentEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Appearing);

		ImGui::Begin("Behavior", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CScriptComponentEditor::show_members()
	{

	}


	CScriptComponentEditor::CScriptComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif