#if MINT_DISTR
#else
#include "AnimatedSpriteComponentEditor.h"

namespace mint::editor
{
	void CAnimatedSpriteComponentEditor::on_terminate()
	{

	}


	void CAnimatedSpriteComponentEditor::on_update(f32 dt)
	{

	}


	void CAnimatedSpriteComponentEditor::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("Editor::ComponentEditor", "CAnimatedSpriteComponentEditor::on_ui_frame");

		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("Animated Sprite", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CAnimatedSpriteComponentEditor::show_members()
	{

	}


	CAnimatedSpriteComponentEditor::CAnimatedSpriteComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif