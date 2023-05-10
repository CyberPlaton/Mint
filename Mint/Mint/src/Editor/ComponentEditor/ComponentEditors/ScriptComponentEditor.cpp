#if MINT_DISTR
#else
#include "ScriptComponentEditor.h"

namespace mint::editor
{
	void CScriptComponentEditor::on_terminate()
	{

	}


	void CScriptComponentEditor::on_update(f32 dt)
	{

	}


	void CScriptComponentEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("Behavior Script", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CScriptComponentEditor::show_members()
	{
		auto entity = get_metaclass()->get_metaclass_entity();
		ImGuiID slid = 10000;
		ImGuiID scid = 20000;
		auto& be = mint::scripting::CBehaviorEngine::Get();

		if(be.does_entity_have_behavior_set(entity))
		{
			auto& behavior = be.get_entity_behavior(entity);

			bool active = behavior.is_active();

			bool checked = ImGui::Checkbox("Active", &active);

			ImGui::SameLine();

			CUI::help_marker("Please note that, while in editing mode all Behaviors are inactive!");

			if (checked)
			{
				behavior.set_active(active);
			}

			ImGui::Text(TextFormat("Entity Behavior: \"%s\"", behavior.get_script_name().c_str()));

			ImGui::SameLine();

			CUI::help_marker(TextFormat("Path: \"%s\"", behavior.get_script_path().c_str()));

			ImGui::SameLine();

			if (ImGui::SmallButton("Detach"))
			{
				be.remove_behavior_from_entity(entity);
			}
		}
		else
		{
			const auto& behaviors = be.get_all_behavior_prefabs();

			for(const auto& script: behaviors)
			{
				bool open = ImGui::Button(script.get_script_name().c_str());

				ImGui::SameLine();

				CUI::help_marker_no_question_mark(script.get_script_path());

				if(open)
				{
					be.set_behavior_for_entity(script.get_script_name(), entity);
				}
			}
		}
	}


	CScriptComponentEditor::CScriptComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif