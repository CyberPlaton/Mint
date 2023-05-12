#if MINT_DISTR
#else
#include "EditorLayerInspectorPanel.h"


namespace mint::editor
{


	bool CInspectorPanelLayer::on_initialize()
	{
		return true;
	}


	void CInspectorPanelLayer::on_update(f32 dt)
	{
		m_componentEditorStack.on_update(dt);
	}


	void CInspectorPanelLayer::on_ui_frame()
	{
		auto width = ImGui::GetWindowWidth();
		auto height = ImGui::GetWindowHeight();

		ImGui::BeginChild("Inspector", { width - 17.5f, percent(height, 98) }, true, get_flags());

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Inspector"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("X"))
			{
				GlobalData().Get().s_EditorInspectedEntity = entt::null;
			}
		}
		ImGui::EndMenuBar();

		show_main_frame();

		if(GlobalData().Get().s_EditorInspectedEntity != entt::null)
		{
			ImGui::Separator();

			if (ImGui::Button(ICON_FA_SQUARE_PLUS " Add Component"))
			{
				m_addingComponent = true;
			}
		}

		ImGui::EndChild();


		if (m_addingComponent) show_add_component_dialog();

		m_componentEditorStack.on_ui_frame();
	}


	mint::String CInspectorPanelLayer::get_layer_name()
	{
		return "CInspectorPanelLayer";
	}


	void CInspectorPanelLayer::show_main_frame()
	{
		auto& db = GlobalData::Get().s_ComponentDatabase;

		String button_text = ICON_FA_TRASH_CAN "##";

		if (GlobalData::Get().s_EditorInspectedEntity != entt::null)
		{
			auto entity_name = CUCA::identifier_get_debug_name(GlobalData::Get().s_EditorInspectedEntity);

			auto& metaclasses = mint::reflection::CEntityMetaclassDatabase::Get().get_entity_metaclasses(SCAST(u64, GlobalData::Get().s_EditorInspectedEntity));

			for (auto& mc : metaclasses)
			{
				if (ImGui::Button(mc->get_metaclass_name().c_str()))
				{
					m_componentEditorStack.try_push_component_editor(mc);
				}

				ImGui::SameLine();

				String text = button_text + mc->get_metaclass_name();
				if (ImGui::Button(text.c_str()))
				{
					db.remove_component_from_entity(mc->get_metaclass_name(), GlobalData::Get().s_EditorInspectedEntity);
				}
			}
		}
	}

	ImGuiWindowFlags CInspectorPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}


	void CInspectorPanelLayer::show_add_component_dialog()
	{
		auto w = GlobalData::Get().s_DefaultEditorTextEditorWidth;
		auto h = GlobalData::Get().s_DefaultEditorTextEditorHeight;
		auto& db = GlobalData::Get().s_ComponentDatabase;
		auto& components = db.get_all_component_names();


		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f, get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Appearing);
		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Appearing);

		String text = "Adding Component to " + CUCA::identifier_get_debug_name(GlobalData::Get().s_EditorInspectedEntity);

		ImGui::Begin(text.c_str(), &m_addingComponent, ImGuiWindowFlags_None);

		for(const auto& comp: components)
		{
			if(ImGui::Button(comp.c_str()))
			{
				db.add_component_to_entity(comp, GlobalData::Get().s_EditorInspectedEntity);
			}
		}

		ImGui::End();
	}


}
#endif