#if MINT_DISTR
#else
#include "SpriteComponentEditor.h"

namespace mint::editor
{
	void CSpriteComponentEditor::on_terminate()
	{

	}


	void CSpriteComponentEditor::on_update(f32 dt)
	{

	}


	void CSpriteComponentEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("Sprite", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CSpriteComponentEditor::show_members()
	{

	}


	CSpriteComponentEditor::CSpriteComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif