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
		MINT_PROFILE_SCOPE("CSceneHierarchyComponentEditor::on_ui_frame", "Editor::ComponentEditor");

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
			
			ImGui::Text(TextFormat("Parent: %s (%zu)", CUCA::identifier_get_debug_name(parent).c_str(), SCAST(u64, parent)));

			show_parent_options(parent, entity);
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
				ImGui::Text(TextFormat("Child: %s (%zu)", CUCA::identifier_get_debug_name(kid).c_str(), SCAST(u64, kid)));

				show_child_options(entity, kid);
			}
		}
	}


	CSceneHierarchyComponentEditor::CSceneHierarchyComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


	void CSceneHierarchyComponentEditor::show_child_options(entt::entity parent, entt::entity child)
	{
		ImGui::SameLine();

		ImGui::PushID(SCAST(s32, child));
		if (ImGui::SmallButton(ICON_FA_CIRCLE_XMARK))
		{
			CUCA::hierarchy_remove_child(parent, child);
		}
		ImGui::PopID();

		ImGui::SameLine();

		ImGui::PushID(SCAST(s32, child));
		if (ImGui::SmallButton(ICON_FA_MAGNIFYING_GLASS))
		{
			GlobalData::Get().s_EditorInspectedEntity = child;
		}
		ImGui::PopID();
	}


	void CSceneHierarchyComponentEditor::show_parent_options(entt::entity parent, entt::entity child)
	{
		ImGui::SameLine();

		ImGui::PushID(SCAST(s32, child));
		if (ImGui::SmallButton(ICON_FA_CIRCLE_XMARK))
		{
			CUCA::hierarchy_remove_parent(child);
		}
		ImGui::PopID();

		ImGui::SameLine();

		ImGui::PushID(SCAST(s32, child));
		if (ImGui::SmallButton(ICON_FA_MAGNIFYING_GLASS))
		{
			GlobalData::Get().s_EditorInspectedEntity = parent;
		}
		ImGui::PopID();
	}


}
#endif