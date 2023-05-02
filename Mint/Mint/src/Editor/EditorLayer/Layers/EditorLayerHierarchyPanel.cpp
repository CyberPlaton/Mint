#include "EditorLayerHierarchyPanel.h"


namespace mint::editor
{


	bool CHierarchyPanelLayer::on_initialize()
	{
		return true;
	}


	void CHierarchyPanelLayer::on_update(f32 dt)
	{
		m_currentSceneName = MINT_ACTIVE_SCENE()->get_scene_name();
	}


	void CHierarchyPanelLayer::on_ui_frame()
	{
		auto width = ImGui::GetWindowWidth();
		auto height = ImGui::GetWindowHeight();

		ImGui::BeginChild("Hierarchy", { width - 17.5f, percent(height, 50) }, true, get_flags());

		if(ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu(lang::get_word(lang::Word_Hierarchy)))
			{
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();

		show_main_frame();

		ImGui::EndChild();
	}


	mint::String CHierarchyPanelLayer::get_layer_name()
	{
		return "Scene Hierarchy Panel";
	}


	ImGuiWindowFlags CHierarchyPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


	void CHierarchyPanelLayer::show_main_frame()
	{
		const auto& entities = MINT_ACTIVE_SCENE()->get_entities();

		ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
		
		const auto open = ImGui::TreeNode(m_currentSceneName.c_str());

		if(open)
		{
			for (const auto& entity : entities)
			{
				if (!CUCA::hierarchy_has_parent(entity))
				{
					show_entity_recursive(entity);
				}
			}

			ImGui::TreePop();
		}
	}


	void CHierarchyPanelLayer::show_entity_recursive(entt::entity entity)
	{
		// Show entity name
		bool inspected = false;
		if(editor::s_EditorInspectedEntity == entity)
		{
			inspected = true;
		}

		if(inspected) ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 0.6f, 0.0f, 1.0f });

		bool open = ImGui::TreeNode(CUCA::identifier_get_debug_name(entity).c_str());

		if (inspected) ImGui::PopStyleColor();

		if(open)
		{
			editor::s_EditorInspectedEntity = entity;

			// Show entities´ children if he has any
			if(CUCA::hierarchy_has_children(entity))
			{
				const auto& children = CUCA::hierarchy_get_children(entity);
				for (const auto& kid : children)
				{
					show_entity_recursive(kid);
				}
			}


			ImGui::TreePop();
		}

	}


}