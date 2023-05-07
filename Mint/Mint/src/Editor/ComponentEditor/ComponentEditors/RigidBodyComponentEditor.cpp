#if MINT_DISTR
#else
#include "RigidBodyComponentEditor.h"

namespace mint::editor
{
	void CRigidBodyComponentEditor::on_terminate()
	{

	}


	void CRigidBodyComponentEditor::on_update(f32 dt)
	{

	}


	void CRigidBodyComponentEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("Rigid Body", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CRigidBodyComponentEditor::show_members()
	{

	}


	CRigidBodyComponentEditor::CRigidBodyComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif