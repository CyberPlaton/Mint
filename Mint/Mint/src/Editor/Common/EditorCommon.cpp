#if MINT_DISTR
#else
#include "EditorCommon.h"


namespace mint::editor
{
	const mint::String GlobalData::s_EditorIconsPath = "EditorRessources/Icons";
	mint::String GlobalData::s_EditorDefaultSceneRessourcesPath = "assets";
	const mint::u32 GlobalData::s_DefaultIconSize = 20;
	const mint::f32 GlobalData::s_DefaultComboWidth = 20.0f;
	const mint::f32 GlobalData::s_DefaultGridcellSize = 64.0f;
	mint::f32 GlobalData::s_DefaultEditorCameraSpeed = 200.0f;
	mint::f32 GlobalData::s_DefaultEditorCameraSpeedDecreased = 20.0f;
	mint::f32 GlobalData::s_DefaultEditorCameraZoomSpeed = 0.5f;
	mint::f32 GlobalData::s_DefaultEditorCameraZoomSpeedDecreased = 0.05f;
	const mint::f32 GlobalData::s_DefaultEditorDialogWidth = 500.0f;
	const mint::f32 GlobalData::s_DefaultEditorDialogHeight = 200.0f;
	const mint::f32 GlobalData::s_DefaultEditorTextEditorWidth = 600.0f;
	const mint::f32 GlobalData::s_DefaultEditorTextEditorHeight = 400.0f;
	const mint::f32 GlobalData::s_DefaultEditorCharacterSize = 13.0f;
	entt::entity GlobalData::s_EditorInspectedEntity = entt::null;
	entt::entity GlobalData::s_EditorOptionSelectedEntity = entt::null;
	entt::entity GlobalData::s_EditorOptionEntityToBeDeleted = entt::null;


	u64 GlobalData::s_EditorTransformMinPosition = -100000 * SCAST(u64, GlobalData::s_DefaultGridcellSize);
	u64 GlobalData::s_EditorTransformMaxPosition = 100000 * SCAST(u64, GlobalData::s_DefaultGridcellSize);
	f32 GlobalData::s_EditorTransformMinScale = 0.01f;
	f32 GlobalData::s_EditorTransformMaxScale = 1000.0f;
	f32 GlobalData::s_EditorTransformMinRotation = -mint::math::pi;
	f32 GlobalData::s_EditorTransformMaxRotation = mint::math::pi;

	mint::editor::CApplicationComponentsDatabase GlobalData::s_ComponentDatabase;
	bool GlobalData::s_EditorProfilerEnabled = false;
	mint::String GlobalData::s_EditorProfilerCategory = "All";


	bool GlobalData::s_EditorBehaviorScriptsEnabled = true;
	bool GlobalData::s_EditorScriptScriptsEnabled = true;
	bool GlobalData::s_EditorAnimationsEnabled = true;


	mint::Vec4 GlobalData::s_EditorWorldQueryAABBColor = {150, 0, 0, 30};
	mint::Vec4 GlobalData::s_EditorSoundSourceCircleColor = { 0, 150, 230 , 30 };
	mint::Vec4 GlobalData::s_EditorSoundSourceMinMaxColor = { 0, 0, 150, 30 };

	bool GlobalData::s_EditorDebugRenderAABBFullInformation = false;
	bool GlobalData::s_EditorDebugRenderAABBs = false;
	bool GlobalData::s_EditorDebugRenderOriginPoint = false;
	bool GlobalData::s_EditorDebugRenderDestinationRect = false;
	bool GlobalData::s_EditorDebugRenderSelected = false;
	bool GlobalData::s_EditorDebugRenderAll = false;
	bool GlobalData::s_EditorDebugRenderSoundSourcePosition = false;
	bool GlobalData::s_EditorDebugRenderSoundSourceMinMaxDistance = false;
	bool GlobalData::s_EditorDebugRenderListenerPosition = false;
	
	mint::Vec3 GlobalData::s_EditorSoundEngineListenerPosition = { 0.0f, 0.0f, 0.0f };



	bool GlobalData::create_asset_file(const String& directory, const String& asset_file_name, const String& asset_file_type, const String& asset_name, const String& asset_source_name, const String& asset_source_type, const String& author, u32 version, const String& description)
	{
		CFilesystem fs(directory);

		if (fs.get_current_directory().does_exist() && fs.get_current_directory().is_directory())
		{
			bool dot_required = !mint::algorithm::string_does_substr_exist(asset_file_type, ".");
			bool file_present = true;

			// Check whether have to create file first or can just open it.
			if (!CFilesystem::find_file(directory, asset_file_name, String(dot_required == true ? "." + asset_file_type : asset_file_type)))
			{
				// Try creating not-existent file.
				file_present = CFilesystem::create_file(directory, asset_file_name, asset_file_type, dot_required);
			}


			// Move "cursor" to newly created file in order to write to it.
			if (file_present && fs.forward(dot_required == true ? String(asset_file_name + "." + asset_file_type) : asset_file_name + asset_file_type))
			{
				String data;
				data += "asset:\n";
				data += "\t" + String("name=\"") + asset_name + "\"\n";

				data += "\t" + String("type=\"") + (dot_required == true ? String("." + asset_file_type) : asset_file_type) + "\"\n";


				// Check whether source requires adding a dot.
				dot_required = !mint::algorithm::string_does_substr_exist(asset_source_type, ".");

				data += "\t" + String("source=\"") + asset_source_name + (dot_required == true ? String("." + asset_source_type) : asset_source_type) + "\"\n";


				data += "\t" + String("author=\"") + author + "\"\n";

				data += "\t" + String("version=") + std::to_string(version) + "\n";

				data += "\t" + String("description=\"") + description + "\"\n";
				data += "end\n";
				data += "\0";


				// Write text data to file.
				return CFileReaderWriter::write_to_file_at_path(fs.get_current_directory().as_string(), (void*)data.c_str(), data.size());
			}
		}

		return false;
	}

	mint::u32 GlobalData::get_initial_sound_source_channel_mode_index(FMOD_MODE mode)
	{
		switch (mode)
		{
		case FMOD_DEFAULT: return 0;
		case FMOD_LOOP_OFF: return 2;
		case FMOD_LOOP_NORMAL: return 3;
		case FMOD_LOOP_BIDI: return 4;
		case FMOD_2D: return 0;
		case FMOD_3D: return 1;
		case FMOD_CREATESTREAM: return 0;
		case FMOD_CREATESAMPLE: return 0;
		case FMOD_CREATECOMPRESSEDSAMPLE: return 0;
		case FMOD_OPENUSER: return 0;
		case FMOD_OPENMEMORY: return 0;
		case FMOD_OPENMEMORY_POINT: return 0;
		case FMOD_OPENRAW: return 0;
		case FMOD_OPENONLY: return 0;
		case FMOD_ACCURATETIME: return 0;
		case FMOD_MPEGSEARCH: return 0;
		case FMOD_NONBLOCKING: return 0;
		case FMOD_UNIQUE: return 0;
		case FMOD_3D_HEADRELATIVE: return 5;
		case FMOD_3D_WORLDRELATIVE: return 6;
		case FMOD_3D_INVERSEROLLOFF: return 7;
		case FMOD_3D_LINEARROLLOFF: return 8;
		case FMOD_3D_LINEARSQUAREROLLOFF: return 9;
		case FMOD_3D_INVERSETAPEREDROLLOFF: return 10;
		case FMOD_3D_CUSTOMROLLOFF: return 0;
		case FMOD_3D_IGNOREGEOMETRY: return 0;
		case FMOD_IGNORETAGS: return 0;
		case FMOD_LOWMEM: return 0;
		case FMOD_VIRTUAL_PLAYFROMSTART: return 0;
		default: return 0;
		}
	}

}
#endif