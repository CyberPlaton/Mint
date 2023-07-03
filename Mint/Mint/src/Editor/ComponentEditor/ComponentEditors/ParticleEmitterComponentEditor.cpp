#if MINT_DISTR
#else
#include "ParticleEmitterComponentEditor.h"


namespace mint::editor
{
	void CParticleEmitterComponentEditor::on_terminate()
	{

	}


	void CParticleEmitterComponentEditor::on_update(f32 dt)
	{

	}


	void CParticleEmitterComponentEditor::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("Editor::ComponentEditor", "CParticleEmitterComponentEditor::on_ui_frame");

		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("ParticleEmitter", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CParticleEmitterComponentEditor::show_members()
	{
		auto entity = get_metaclass()->get_metaclass_entity();
		auto gd = GlobalData::Get();
		ImGuiID slid = 10000;
		ImGuiID scid = 20000;

		auto emitter = mint::fx::CParticleSystem::Get().get_particle_emitter_for_entity(entity);
		if (emitter)
		{
			auto& def = emitter->get_particle_definition();

			static f32 loading = 0.0f;
			loading += 0.01f;
			if (loading >= 100.0f) loading = 0.0f;

			CUI::loading_bar("Loading", loading, { 250.0f, 10.0f }, MINT_GREY_DARK(), MINT_GREEN());
			CUI::loading_circle("Loading", 10.0f, 5.0f, MINT_GREEN());

			ImGui::SeparatorText("Position");

			CUI::edit_field_vec2(def.m_positionStart, 0.0f, 1000.0f, "Position Start", "", slid++, scid++);
			CUI::edit_field_vec2(def.m_positionEnd, 0.0f, 1000.0f, "Position End", "", slid++, scid++);
			CUI::edit_field_vec2_ranged(def.m_positionOffset, 0.0f, 0.0f, "Position Offset", "", slid++, scid++);
			

			ImGui::SeparatorText("Velocity");

			CUI::edit_field_vec2(def.m_velocity, 0.0f, 1000.0f, "Velocity Direction", "", slid++, scid++);
			CUI::edit_field_vec2_ranged(def.m_velocityOffset, 0.0f, 0.0f, "Velocity Offset", "", slid++, scid++);
		

			ImGui::SeparatorText("Scale");

			CUI::edit_field_vec2(def.m_scaleStart, 0.0f, 1000.0f, "Scale Start", "", slid++, scid++);
			CUI::edit_field_vec2(def.m_scaleEnd, 0.0f, 1000.0f, "Scale End", "", slid++, scid++);
			CUI::edit_field_vec2_ranged(def.m_scaleOffset, 0.0f, 0.0f, "Scale Offset", "", slid++, scid++);
		}
	}


	CParticleEmitterComponentEditor::CParticleEmitterComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif