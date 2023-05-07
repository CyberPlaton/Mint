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

		ImGui::EndChild();

		m_componentEditorStack.on_ui_frame();
	}


	mint::String CInspectorPanelLayer::get_layer_name()
	{
		return "Entity Inspector Panel";
	}


	void CInspectorPanelLayer::show_main_frame()
	{
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
			}
		}
	}

	ImGuiWindowFlags CInspectorPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	}

}
#endif