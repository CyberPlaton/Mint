#if MINT_DISTR
#else
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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu(lang::get_word(lang::Word_Hierarchy)))
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

		if (open)
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
		if (GlobalData::Get().s_EditorInspectedEntity == entity)
		{
			inspected = true;
		}

		if (inspected)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.25f, 0.5f, 1.0f, 1.0f });
		}

		bool open = ImGui::TreeNode(CUCA::identifier_get_debug_name(entity).c_str());

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			GlobalData::Get().s_EditorInspectedEntity = entity;
		}

		check_entity_for_components_sanity(entity);

		if (inspected) ImGui::PopStyleColor();

		if (open)
		{
			// Show entities´ children if he has any
			if (CUCA::hierarchy_has_children(entity))
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


	void CHierarchyPanelLayer::check_entity_for_components_sanity(entt::entity entity)
	{
		auto& registry = MINT_ACTIVE_SCENE()->get_registry();

		bool info = false;
		String info_message;
		u32 info_count = 1;

		if(registry.has_component< mint::component::STransform >(entity))
		{
			if(!registry.has_component< mint::component::SDynamicGameobject >(entity))
			{
				info_message = std::to_string(info_count) + ".) The entity does not have a SDynamicGameobject "
															"Component, and its position will not be updated when it changes its Position!";

				info = true;
				info_count++;
			}
		}

		if (registry.has_component< mint::component::SRigidBody >(entity))
		{
			if (!registry.has_component< mint::component::STransform >(entity))
			{
				info_message = std::to_string(info_count) + ".) The entity does not have a STransform "
														    "Component, and will not be functioning without it. Either remove "
															"SRigidBody Component or add STransform Component!";

				info = true;
				info_count++;
			}
		}

		if (registry.has_component< mint::component::SSprite >(entity))
		{
			if (!registry.has_component< mint::component::STransform >(entity))
			{
				info_message = std::to_string(info_count) + ".) The entity does not have a STransform "
														    "Component, and will not be rendered! Either remove "
															"SSprite Component or add STransform Component!";

				info = true;
				info_count++;
			}
		}

		if (registry.has_component< mint::component::SAnimatedSprite >(entity))
		{
			if (!registry.has_component< mint::component::SSprite >(entity))
			{
				info_message = std::to_string(info_count) + ".) The entity does not have a SSprite "
															"Component, and will not be rendered nor animated! Either remove "
															"SAnimatedSprite Component or add SSprite Component!";

				info = true;
				info_count++;
			}
		}

		ImGui::SameLine();

		ImGui::Text(ICON_FA_CIRCLE_EXCLAMATION);

		if(ImGui::IsItemHovered())
		{
			CUI::help_marker_no_question_mark(info_message);
		}

	}


}
#endif