#if MINT_DISTR
#else
#include "SoundSourceComponentEditor.h"

namespace mint::editor
{
	void CSoundSourceComponentEditor::on_terminate()
	{

	}


	void CSoundSourceComponentEditor::on_update(f32 dt)
	{

	}


	void CSoundSourceComponentEditor::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("Editor::ComponentEditor", "CSoundSourceComponentEditor::on_ui_frame");

		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("Sound Source", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CSoundSourceComponentEditor::show_members()
	{
		auto entity = get_metaclass()->get_metaclass_entity();
		ImGuiID slid = 10000;
		ImGuiID scid = 20000;

		static s32 selected_fmod_sound_source_mode = -1;
		const auto& soundsource = MINT_SCENE_REGISTRY()->get_component< mint::component::SSoundSource >(entity);

		auto enabled = CUCA::world_get_entity_enabled(entity);
		auto handle = CUCA::soundsource_get_sound_source_sound_handle(entity);
		selected_fmod_sound_source_mode = CUCA::soundsource_get_sound_source_mode(entity);
		auto loopmode = CUCA::soundsource_get_sound_source_loopmode(entity);
		auto volume = CUCA::soundsource_get_sound_source_volume(entity) * 100.0f; // Volume is from 0.0f to 1.0f.
		auto min_dist = soundsource.m_minDistance;
		auto max_dist = soundsource.m_maxDistance;
		auto pitch = CUCA::soundsource_get_sound_source_pitch(entity);
		auto pan = CUCA::soundsource_get_sound_source_pan(entity);
		auto position = CUCA::transform_get_position(entity);
		auto height = CUCA::soundsource_get_sound_source_height(entity);
		auto velocity = CUCA::soundsource_get_sound_source_velocity(entity);
		auto cone_orient = CUCA::soundsource_get_sound_source_cone_orientation(entity);
		auto cone_settings = CUCA::soundsource_get_sound_source_cone_settings(entity);



		CUI::toggle_button("Enabled", &enabled);

		if (ImGui::Button(ICON_FA_PLAY " Play"))
		{
			sound::CSoundEngine::Get().play_sound(entity);
		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_PAUSE " Toggle paused"))
		{
			sound::CSoundEngine::Get().toggle_pause_sound(entity);
		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_STOP " Stop"))
		{
			sound::CSoundEngine::Get().stop_sound(entity);
		}



		ImGui::Separator();

		ImGuiKnobs::Knob("Volume", &volume, 0.0f, 100.0f, 0.1f, "%.3f", ImGuiKnobVariant_Stepped, 50.0f, ImGuiKnobFlags_DragHorizontal, 1);

		ImGui::SameLine();

		ImGuiKnobs::Knob("Pitch", &pitch, -1.0f, 1.0f, 0.01f, "%.3f", ImGuiKnobVariant_Stepped, 50.0f, ImGuiKnobFlags_DragHorizontal, 1);

		ImGui::SameLine();

		ImGuiKnobs::Knob("Pan", &pan, -1.0f, 1.0f, 0.01f, "%.3f", ImGuiKnobVariant_Stepped, 50.0f, ImGuiKnobFlags_DragHorizontal, 1);

		ImGui::Separator();

		CUI::edit_field_vec2(position, GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, "Position", "", slid++, scid++, ImGuiSliderFlags_Logarithmic);
		CUI::edit_field_f32(height, -100.0f, 100.0f, "Height", "", slid++, scid++, ImGuiSliderFlags_Logarithmic);
		CUI::edit_field_vec3(velocity, -1000.0f, 1000.0f, "Velocity", "", slid++, scid++, ImGuiSliderFlags_Logarithmic);
		
		CUI::edit_field_vec3(cone_orient, -1.0f, 1.0f, "Cone orientation", "", slid++, scid++, ImGuiSliderFlags_Logarithmic);

		Vec2 cone_sett = { cone_settings.x, cone_settings.y };
		f32 cone_outer = cone_settings.z;
		CUI::edit_field_vec2_ranged(cone_sett, 0.0f, 360.0f, "Cone settings", "", slid++, scid++, ImGuiSliderFlags_Logarithmic, 0.5f);
		CUI::edit_field_f32(cone_outer, 0.0f, 1.0f, "Outer cone volume", "", slid++, scid++, 0, 0.05f);

		ImGui::Separator();

		Vec2 distance = { min_dist, max_dist };
		CUI::edit_field_vec2_ranged(distance, 0.0f, 10000.0f, "Distance", "", slid++, scid++, ImGuiSliderFlags_Logarithmic);

		if (ImGui::Button("FMOD Sound source mode"))
		{
			ImGui::OpenPopup("FMOD_Mode_Popup");
		}

		if (ImGui::BeginPopup("FMOD_Mode_Popup"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(SEditorSoundSourceChannelModes); i++)
			{
				if (ImGui::Selectable(SEditorSoundSourceChannelModes[i])) { selected_fmod_sound_source_mode = i; }
			}

			ImGui::EndPopup();

			CUCA::soundsource_set_sound_source_mode(entity, (sound::SoundSourceChannelMode)selected_fmod_sound_source_mode);
		}


		CUCA::soundsource_set_sound_source_pan(entity, pan);
		CUCA::soundsource_set_sound_source_volume(entity, volume / 100.0f);
		CUCA::soundsource_set_sound_source_pitch(entity, pitch);
		CUCA::world_set_entity_enabled(entity, enabled);
		CUCA::soundsource_set_sound_source_velocity(entity, velocity);
		CUCA::soundsource_set_sound_source_cone_orientation(entity, cone_orient);
		CUCA::soundsource_set_sound_source_cone_settings(entity, cone_sett.x, cone_sett.y, cone_outer);
		CUCA::soundsource_set_sound_source_height(entity, height);
		CUCA::transform_set_position(entity, position);
	}


	CSoundSourceComponentEditor::CSoundSourceComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif