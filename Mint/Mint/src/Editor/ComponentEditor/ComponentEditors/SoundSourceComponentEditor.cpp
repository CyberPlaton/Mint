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
		auto min_dist = CUCA::soundsource_get_sound_source_min_distance(entity);
		auto max_dist = CUCA::soundsource_get_sound_source_max_distance(entity);
		Vec2 distance = { min_dist, max_dist };
		auto pitch = CUCA::soundsource_get_sound_source_pitch(entity);
		auto pan = CUCA::soundsource_get_sound_source_pan(entity);
		auto height = CUCA::soundsource_get_sound_source_height(entity);
		auto velocity = CUCA::soundsource_get_sound_source_velocity(entity);



		CUI::toggle_button("Enabled", &enabled);

		ImGui::SameLine();

		if (sound::CSoundEngine::Get().is_sound_source_virtual(entity))
		{
			ImGui::SmallButton(ICON_FA_EAR_DEAF " Virtual");
		}
		else
		{
			ImGui::SmallButton(ICON_FA_EAR_LISTEN " Not virtual");
		}

		ImGui::SameLine();

		ImGui::SmallButton(TextFormat("Size: %dKB", sound::CSoundEngine::Get().get_sound_source_size_in_bytes(handle) / 1000));




		if (ImGui::Button(ICON_FA_PLAY " Play"))
		{
			sound::CSoundEngine::Get().play_sound(entity);
		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_PAUSE " Pause"))
		{
			sound::CSoundEngine::Get().toggle_pause_sound(entity);
		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_STOP " Stop"))
		{
			sound::CSoundEngine::Get().stop_sound(entity);
		}

		auto length_minutes = sound::CSoundEngine::Get().get_sound_length_minutes(entity);
		auto length_seconds = sound::CSoundEngine::Get().get_sound_length_seconds(entity);
		auto length_ms = sound::CSoundEngine::Get().get_sound_length_milliseconds(entity);
		auto full_length = sound::CSoundEngine::Get().get_sound_length(entity);

		auto position_minutes = sound::CSoundEngine::Get().get_sound_position_minutes(entity);
		auto position_seconds = sound::CSoundEngine::Get().get_sound_position_seconds(entity);
		auto position_ms = sound::CSoundEngine::Get().get_sound_position_milliseconds(entity);
		auto full_position = sound::CSoundEngine::Get().get_sound_position(entity);

		ImGui::Text(TextFormat("%d:%d:%d/%d:%d:%d", position_minutes, position_seconds, position_ms, length_minutes, length_seconds, length_ms));

		ImGui::Separator();

		ImGuiKnobs::Knob("Volume", &volume, 0.0f, 100.0f, 0.1f, "%.3f", ImGuiKnobVariant_Stepped, 50.0f, ImGuiKnobFlags_DragHorizontal, 1);

		ImGui::SameLine();

		ImGuiKnobs::Knob("Pitch", &pitch, -1.0f, 1.0f, 0.01f, "%.3f", ImGuiKnobVariant_Stepped, 50.0f, ImGuiKnobFlags_DragHorizontal, 1);

		ImGui::SameLine();

		ImGuiKnobs::Knob("Pan", &pan, -1.0f, 1.0f, 0.01f, "%.3f", ImGuiKnobVariant_Stepped, 50.0f, ImGuiKnobFlags_DragHorizontal, 1);

		ImGui::Separator();

		CUI::edit_field_f32(height, -100.0f, 100.0f, "Height", GlobalData::Get().s_SoundSourceDescHeight, slid++, scid++, ImGuiSliderFlags_Logarithmic);
		CUI::edit_field_vec3(velocity, -1000.0f, 1000.0f, "Velocity", GlobalData::Get().s_SoundSourceDescVelocity, slid++, scid++, ImGuiSliderFlags_Logarithmic);
		
		ImGui::Separator();

		CUI::edit_field_vec2_ranged(distance, 0.0f, 5000.0f, "Distance", GlobalData::Get().s_SoundSourceDescMinMaxDistance, slid++, scid++, ImGuiSliderFlags_Logarithmic, 2.0f);


		ImGui::Text("FMOD Mode: " STRING(CUCA::soundsource_get_sound_source_mode(entity)));


// 		String text = "FMOD Mode: " + String(SEditorSoundSourceChannelModes[selected_fmod_sound_source_mode]);
// 
// 		if (ImGui::Button(text.c_str()))
// 		{
// 			ImGui::OpenPopup("FMOD_Mode_Popup");
// 		}
// 
// 		if (ImGui::BeginPopup("FMOD_Mode_Popup"))
// 		{
// 			for (int i = 0; i < IM_ARRAYSIZE(SEditorSoundSourceChannelModes); i++)
// 			{
// 				if (ImGui::Selectable(SEditorSoundSourceChannelModes[i])) { selected_fmod_sound_source_mode = i; }
// 			}
// 
// 			ImGui::EndPopup();
// 
// 			CUCA::soundsource_set_sound_source_mode(entity, (sound::SoundSourceChannelMode)selected_fmod_sound_source_mode);
// 			sound::CSoundEngine::Get().set_sound_source_mode(entity, (sound::SoundSourceChannelMode)selected_fmod_sound_source_mode);
// 		}


		CUCA::soundsource_set_sound_source_pan(entity, pan);
		CUCA::soundsource_set_sound_source_volume(entity, volume / 100.0f);
		CUCA::soundsource_set_sound_source_pitch(entity, pitch);
		CUCA::world_set_entity_enabled(entity, enabled);
		CUCA::soundsource_set_sound_source_velocity(entity, velocity);
		CUCA::soundsource_set_sound_source_height(entity, height);
		CUCA::soundsource_set_sound_source_min_distance(entity, distance.x);
		CUCA::soundsource_set_sound_source_max_distance(entity, distance.y);

		//sound::CSoundEngine::Get().set_sound_source_pan(entity, pan);
		//sound::CSoundEngine::Get().set_sound_source_pitch(entity, pitch);
		//sound::CSoundEngine::Get().set_sound_source_volume(entity, volume);
		//sound::CSoundEngine::Get().set_sound_source_position_and_velocity(entity, { position.x, position.y, height }, velocity);
		//sound::CSoundEngine::Get().set_sound_source_cone_orientation(entity, cone_orient);
		//sound::CSoundEngine::Get().set_sound_source_cone_settings(entity, cone_sett.x, cone_sett.y, cone_outer);
		sound::CSoundEngine::Get().set_sound_source_min_and_max_distance(entity, distance.x, distance.y);
	}


	CSoundSourceComponentEditor::CSoundSourceComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif