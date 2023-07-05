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
			f32 particles_rate = emitter->get_particles_emission_rate();

			CUI::edit_field_f32(particles_rate, 0.0f, 100.0f, "Particles emission rate", "", slid++, scid++, ImGuiSliderFlags_None, 0.01f);

			emitter->set_particles_emission_rate(particles_rate);


			static s32 selected_tangent_easing_option = (s32)emitter->get_tangential_velocity_ease();
			static s32 selected_angular_easing_option = (s32)emitter->get_angular_velocity_ease();


			ImGui::Separator();

			auto& def = emitter->get_particle_definition();

			ImGui::BeginTabBar("Tab");

			if (ImGui::BeginTabItem("Transform"))
			{
				CUI::edit_field_vec2(def.m_positionStart, 0.0f, 1000.0f, "Position", "", slid++, scid++);
				CUI::edit_field_f32(def.m_rotation, GlobalData::Get().s_EditorTransformMinRotation, GlobalData::Get().s_EditorTransformMaxRotation, "Rotation", "", slid++, scid++, ImGuiSliderFlags_None, 0.01f);
				CUI::edit_field_vec2_ranged(def.m_positionStartOffset, -100.0f, 100.0f, "Position Offset", "", slid++, scid++);
				CUI::edit_field_vec2_ranged(def.m_rotationOffset, GlobalData::Get().s_EditorTransformMinRotation, GlobalData::Get().s_EditorTransformMaxRotation, "Rotation Offset", "", slid++, scid++, ImGuiSliderFlags_None, 0.01f);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Movement"))
			{
				CUI::edit_field_f32(def.m_tangentialVelocity, -500.0f, 500.0f, "Tangential Velocity", "", slid++, scid++, ImGuiSliderFlags_None, 1.0f);
				CUI::edit_field_f32(def.m_angularVelocity, -500.0f, 500.0f, "Angular Velocity", "", slid++, scid++, ImGuiSliderFlags_None, 1.0f);
				CUI::edit_field_vec2(def.m_startingDirection, -1.0f, 1.0f, "Start Looking Direction", "", slid++, scid++, ImGuiSliderFlags_None, 0.01f);
				CUI::edit_field_vec2_ranged(def.m_startingDirectionOffset, -100.0f, 100.0f, "Direction Offset", "", slid++, scid++);
				CUI::edit_field_vec2_ranged(def.m_tangentialVelocityOffset, -100.0f, 100.0f, "Tangential Velocity Offset", "", slid++, scid++);
				CUI::edit_field_vec2_ranged(def.m_angularVelocityOffset, -100.0f, 100.0f, "Angular Velocity Offset", "", slid++, scid++);
				
				if (CUI::edit_field_vec2_ranged(def.m_tangentialVelocityFalloff, -100.0f, 100.0f, "Tangential Velocity Falloff", "", slid++, scid++))
				{
					if (ImGui::SmallButton(TextFormat("Tang. Easing mode: %s", s_EditorEasingOptions[selected_tangent_easing_option])))
					{
						ImGui::OpenPopup("Tangential_Easing_Popup");
					}
				}
			
				if (CUI::edit_field_vec2_ranged(def.m_angularVelocityFalloff, -100.0f, 100.0f, "Angular Velocity Falloff", "", slid++, scid++))
				{
					if (ImGui::SmallButton(TextFormat("Ang. Easing mode: %s", s_EditorEasingOptions[selected_tangent_easing_option])))
					{
						ImGui::OpenPopup("Angular_Easing_Popup");
					}
				}


				if (ImGui::BeginPopup("Tangential_Easing_Popup"))
				{
					for (int i = 0; i < IM_ARRAYSIZE(s_EditorEasingOptions); i++)
					{
						if (ImGui::Selectable(s_EditorEasingOptions[i])) { selected_tangent_easing_option = i; }
					}

					ImGui::EndPopup();

					emitter->set_tangential_velocity_ease((bx::Easing::Enum)selected_tangent_easing_option);
				}


				if (ImGui::BeginPopup("Angular_Easing_Popup"))
				{
					for (int i = 0; i < IM_ARRAYSIZE(s_EditorEasingOptions); i++)
					{
						if (ImGui::Selectable(s_EditorEasingOptions[i])) { selected_angular_easing_option = i; }
					}

					ImGui::EndPopup();

					emitter->set_angular_velocity_ease((bx::Easing::Enum)selected_angular_easing_option);
				}


				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Scale"))
			{
				CUI::edit_field_vec2(def.m_scaleStart, 0.0f, 1000.0f, "Start", "", slid++, scid++);
				CUI::edit_field_vec2(def.m_scaleEnd, 0.0f, 1000.0f, "End", "", slid++, scid++);
				CUI::edit_field_vec2_ranged(def.m_scaleOffset, 0.0f, 0.0f, "Offset", "", slid++, scid++);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Lifetime"))
			{
				CUI::edit_field_f32(def.m_lifespan, 1.0f, 1000.0f, "Time", "", slid++, scid++);
				CUI::edit_field_vec2_ranged(def.m_lifespanOffset, 0.0f, 0.0f, "Offset", "", slid++, scid++);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Color"))
			{
				CUI::edit_field_color(def.m_colorStart, 0, 255, "Start", "", slid++, scid++);
				CUI::edit_field_color(def.m_colorEnd, 0, 255, "End", "", slid++, scid++);
				CUI::edit_field_vec2_ranged(def.m_colorOffset, 0.0f, 255.0f, "Offset", "", slid++, scid++);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}


	CParticleEmitterComponentEditor::CParticleEmitterComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif