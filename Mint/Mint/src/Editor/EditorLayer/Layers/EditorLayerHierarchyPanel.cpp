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

		ImGui::BeginChild(lang::get_word(lang::Word_Hierarchy), { width - 17.5f, percent(height, 50) }, true, get_flags());

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Scene"))
			{
				if(ImGui::MenuItem("Reload"))
				{
					auto scene = MINT_ACTIVE_SCENE();
					auto name = scene->get_scene_name();
					scene->transit_to_scene(name);
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();

		show_main_frame();

		ImGui::EndChild();
	}


	mint::String CHierarchyPanelLayer::get_layer_name()
	{
		return "CHierarchyPanelLayer";
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
		auto name = CUCA::identifier_get_debug_name(entity);
		bool inspected = false;
		if (GlobalData::Get().s_EditorInspectedEntity == entity)
		{
			inspected = true;
		}

		if (inspected)
		{
			// Selection outline.
			auto drawList = ImGui::GetWindowDrawList();
			auto cursor = ImGui::GetCursorScreenPos();
			auto parent_size = ImGui::GetItemRectSize();
			auto content_region = ImGui::GetContentRegionAvail();

			ImVec2 vstart = { cursor.x, cursor.y };
			ImVec2 vend = { vstart.x + content_region.x, vstart.y + parent_size.y };

			drawList->AddRectFilled(vstart, vend, ImGui::ColorConvertFloat4ToU32({ 0.47f, 0.77f, 1.0f, 0.4f }));

			// Entity name coloring.
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.9f, 0.9f, 1.0f });
		}

		bool open = ImGui::TreeNode(name.c_str());

		if (inspected) ImGui::PopStyleColor();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			GlobalData::Get().s_EditorInspectedEntity = entity;
		}



		ImGui::SameLine();

		static s32 selected_entity_option = -1;

		ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.9f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
		if(ImGui::SmallButton(ICON_FA_GEAR))
		{
			ImGui::OpenPopup("Entity Options Popup", ImGuiPopupFlags_MouseButtonRight);
			GlobalData::Get().s_EditorOptionSelectedEntity = entity;
		}
		ImGui::PopStyleColor(2);

		if (ImGui::BeginPopup("Entity Options Popup"))
		{
			ImGui::SeparatorText("Entity Options");

			for (int i = 0; i < IM_ARRAYSIZE(s_EditorEntityOptions); i++)
				if (ImGui::Selectable(s_EditorEntityOptions[i]))
					selected_entity_option = i;

			ImGui::EndPopup();
		}

		check_entity_for_components_sanity(entity);

		react_to_selected_entity_option(&selected_entity_option);

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

		bool warning = false;
		String info_message;
		u32 info_count = 1;

		if(registry.has_component< mint::component::STransform >(entity))
		{
			if(!registry.has_component< mint::component::SDynamicGameobject >(entity))
			{
				info_message += std::to_string(info_count) + ".) The entity does not have a SDynamicGameobject "
															"Component, and its position will not be updated when it changes its Position!";

				info_message += "\n";

				warning = true;
				info_count++;
			}
		}

		if (registry.has_component< mint::component::SRigidBody >(entity))
		{
			if (!registry.has_component< mint::component::STransform >(entity))
			{
				info_message += std::to_string(info_count) + ".) The entity does not have a STransform "
														    "Component, and will not be functioning without it. Either remove "
															"SRigidBody Component or add STransform Component!";

				info_message += "\n";

				warning = true;
				info_count++;
			}
		}

		if (registry.has_component< mint::component::SSprite >(entity))
		{
			if (!registry.has_component< mint::component::STransform >(entity))
			{
				info_message += std::to_string(info_count) + ".) The entity does not have a STransform "
														    "Component, and will not be rendered! Either remove "
															"SSprite Component or add STransform Component!";

				info_message += "\n";

				warning = true;
				info_count++;
			}
		}

		if (registry.has_component< mint::component::SAnimatedSprite >(entity))
		{
			if (!registry.has_component< mint::component::SSprite >(entity))
			{
				info_message += std::to_string(info_count) + ".) The entity does not have a SSprite "
															"Component, and will not be rendered nor animated! Either remove "
															"SAnimatedSprite Component or add SSprite Component!";

				info_message += "\n";

				warning = true;
				info_count++;
			}
		}


		if(warning)
		{
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32({ 1.0f, 0.57f, 0.0f, 1.0f }));

			ImGui::Text(ICON_FA_CIRCLE_EXCLAMATION " Warning");

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered())
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32({ 1.0f, 0.57f, 0.0f, 1.0f }));

				CUI::help_marker_no_question_mark(info_message);

				ImGui::PopStyleColor();
			}
		}
	}


	void CHierarchyPanelLayer::react_to_selected_entity_option(s32* option)
	{
		if (*option == -1) return;

		auto op = *option;
		
		switch(op)
		{
		case 0: create_dynamic_child_entity(GlobalData::Get().s_EditorOptionSelectedEntity); break;
		case 1: create_dynamic_parent_entity(GlobalData::Get().s_EditorOptionSelectedEntity); break;
		case 2: create_static_child_entity(GlobalData::Get().s_EditorOptionSelectedEntity); break;
		case 3: create_static_parent_entity(GlobalData::Get().s_EditorOptionSelectedEntity); break;
		}

		*option = -1;
		GlobalData::Get().s_EditorOptionSelectedEntity = entt::null;
	}


	void CHierarchyPanelLayer::create_dynamic_child_entity(entt::entity parent)
	{
		auto scene = MINT_ACTIVE_SCENE();
		auto& registry = scene->get_registry();


		auto entity = registry.create_entity();

		auto& identifier = registry.add_component< mint::component::SIdentifier >(entity);
		auto& hierarchy = registry.add_component< mint::component::SSceneHierarchy >(entity);
		auto& transform = registry.add_component< mint::component::STransform >(entity);
		auto& dynamic = registry.add_component< mint::component::SDynamicGameobject >(entity);

		identifier.m_enttId = SCAST(u64, entity);
		identifier.m_uuid = identifier.m_enttId;
		identifier.m_debugName = "EmptyDynamicEntity_" + std::to_string(identifier.m_enttId);
		
		CUCA::hierarchy_set_parent(entity, parent);

		CUCA::hierarchy_add_child(parent, entity);

		CUCA::transform_set_scale(entity, { 1.0f, 1.0f });
		CUCA::transform_set_rotation(entity, 0.0f);
		CUCA::transform_set_position(entity, { 1.0f, 1.0f });

		scene->add_entity(entity);
	}


	void CHierarchyPanelLayer::create_dynamic_parent_entity(entt::entity child)
	{
		auto scene = MINT_ACTIVE_SCENE();
		auto& registry = scene->get_registry();


		auto entity = registry.create_entity();

		auto& identifier = registry.add_component< mint::component::SIdentifier >(entity);
		auto& hierarchy = registry.add_component< mint::component::SSceneHierarchy >(entity);
		auto& transform = registry.add_component< mint::component::STransform >(entity);
		auto& dynamic = registry.add_component< mint::component::SDynamicGameobject >(entity);

		identifier.m_enttId = SCAST(u64, entity);
		identifier.m_uuid = identifier.m_enttId;
		identifier.m_debugName = "EmptyDynamicEntity_" + std::to_string(identifier.m_enttId);
		hierarchy.m_parent = entt::null;

		CUCA::hierarchy_set_parent(child, entity);

		CUCA::hierarchy_add_child(entity, child);

		CUCA::transform_set_scale(entity, { 1.0f, 1.0f });
		CUCA::transform_set_rotation(entity, 0.0f);
		CUCA::transform_set_position(entity, { 1.0f, 1.0f });

		scene->add_entity(entity);
	}


	void CHierarchyPanelLayer::create_static_child_entity(entt::entity parent)
	{
		auto scene = MINT_ACTIVE_SCENE();
		auto& registry = scene->get_registry();


		auto entity = registry.create_entity();

		auto& identifier = registry.add_component< mint::component::SIdentifier >(entity);
		auto& hierarchy = registry.add_component< mint::component::SSceneHierarchy >(entity);
		auto& transform = registry.add_component< mint::component::STransform >(entity);

		identifier.m_enttId = SCAST(u64, entity);
		identifier.m_uuid = identifier.m_enttId;
		identifier.m_debugName = "EmptyStaticEntity_" + std::to_string(identifier.m_enttId);
		hierarchy.m_parent = parent;

		CUCA::transform_set_scale(entity, { 1.0f, 1.0f });
		CUCA::transform_set_rotation(entity, 0.0f);
		CUCA::transform_set_position(entity, { 1.0f, 1.0f });

		scene->add_entity(entity);

		CSAS::Get().submit_scene_static_entities(scene->get_entities());
	}


	void CHierarchyPanelLayer::create_static_parent_entity(entt::entity child)
	{
		auto scene = MINT_ACTIVE_SCENE();
		auto& registry = scene->get_registry();


		auto entity = registry.create_entity();

		auto& identifier = registry.add_component< mint::component::SIdentifier >(entity);
		auto& hierarchy = registry.add_component< mint::component::SSceneHierarchy >(entity);
		auto& transform = registry.add_component< mint::component::STransform >(entity);

		identifier.m_enttId = SCAST(u64, entity);
		identifier.m_uuid = identifier.m_enttId;
		identifier.m_debugName = "EmptyStaticEntity_" + std::to_string(identifier.m_enttId);
		hierarchy.m_parent = entt::null;

		CUCA::hierarchy_set_parent(child, entity);

		CUCA::transform_set_scale(entity, { 1.0f, 1.0f });
		CUCA::transform_set_rotation(entity, 0.0f);
		CUCA::transform_set_position(entity, { 1.0f, 1.0f });

		scene->add_entity(entity);

		CSAS::Get().submit_scene_static_entities(scene->get_entities());
	}


}
#endif