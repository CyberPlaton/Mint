#if MINT_DISTR
#else
#include "SceneHierarchyComponentEditor.h"

namespace mint::editor
{
	void CSceneHierarchyComponentEditor::on_terminate()
	{

	}


	void CSceneHierarchyComponentEditor::on_update(f32 dt)
	{

	}


	void CSceneHierarchyComponentEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("Scene Hierarchy", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CSceneHierarchyComponentEditor::show_members()
	{
		auto entity = get_metaclass()->get_metaclass_entity();
		ImGuiID slid = 10000;
		ImGuiID scid = 20000;


		auto parent = CUCA::hierarchy_get_parent(entity);
		auto& children = CUCA::hierarchy_get_children(entity);


		if(parent == entt::null)
		{
			ImGui::Text("No Parent");
		}
		else
		{
			ImGui::Text(TextFormat("Parent: %zu", SCAST(u64, parent)));
		}

		ImGui::Separator();

		if(children.empty())
		{
			ImGui::Text("No Children");
		}
		else
		{
			ImGui::Text("Children:");
			for(auto& kid: children)
			{
				ImGui::Text(TextFormat("Child: %zu", SCAST(u64, kid)));
			}
		}
	}


	CSceneHierarchyComponentEditor::CSceneHierarchyComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif