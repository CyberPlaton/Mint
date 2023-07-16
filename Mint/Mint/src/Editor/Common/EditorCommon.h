#if MINT_DISTR
#else
#ifndef _MINTEDITOR_EDITOR_COMMON_H_
#define _MINTEDITOR_EDITOR_COMMON_H_



#include "UI/UI.h"
#include "../../Editor/ComponentEditor/ApplicationComponentsDatabase.h"
#include "Common/Math.h"
#include "Utility/FileSystem/FileIO.h"



namespace mint::editor
{
	
	struct GlobalData
	{
		STATIC_GET(GlobalData, s_GlobalData);

		static bool create_asset_file(const String& directory, const String& asset_file_name, const String& asset_file_type,
					const String& asset_name, const String& asset_source_name, const String& asset_source_type,
					const String& author, u32 version, const String& description);





		
		static bool s_EditorDebugRenderAll;
		static bool s_EditorDebugRenderSelected;
		
		static bool s_EditorDebugRenderDestinationRect;
		static bool s_EditorDebugRenderOriginPoint;

		static bool s_EditorDebugRenderAABBs;
		static bool s_EditorDebugRenderAABBFullInformation;

		static bool s_EditorDebugRenderListenerPosition;
		static bool s_EditorDebugRenderSoundSourcePosition;
		static bool s_EditorDebugRenderSoundSourceMinMaxDistance;

		static bool s_EditorProfilerEnabled;
		static bool s_EditorBehaviorScriptsEnabled;
		static bool s_EditorScriptScriptsEnabled;
		static bool s_EditorAnimationsEnabled;

		static mint::String s_EditorProfilerCategory;
		static const mint::String s_EditorIconsPath;
		static mint::String s_EditorDefaultSceneRessourcesPath;
		static const mint::u32 s_DefaultIconSize;
		static const mint::f32 s_DefaultComboWidth;
		static const mint::f32 s_DefaultGridcellSize;
		static mint::f32 s_DefaultEditorCameraSpeed;
		static mint::f32 s_DefaultEditorCameraSpeedDecreased;
		static mint::f32 s_DefaultEditorCameraZoomSpeed;
		static mint::f32 s_DefaultEditorCameraZoomSpeedDecreased;
		static const mint::f32 s_DefaultEditorDialogWidth;
		static const mint::f32 s_DefaultEditorDialogHeight;
		static const mint::f32 s_DefaultEditorTextEditorWidth;
		static const mint::f32 s_DefaultEditorTextEditorHeight;
		static const mint::f32 s_DefaultEditorCharacterSize;
		static entt::entity s_EditorInspectedEntity;
		static entt::entity s_EditorOptionSelectedEntity;
		static entt::entity s_EditorOptionEntityToBeDeleted;


		static u64 s_EditorTransformMinPosition;
		static u64 s_EditorTransformMaxPosition;
		static f32 s_EditorTransformMinScale;
		static f32 s_EditorTransformMaxScale;
		static f32 s_EditorTransformMinRotation;
		static f32 s_EditorTransformMaxRotation;

		static Vec4 s_EditorWorldQueryAABBColor;
		static Vec4 s_EditorSoundSourceCircleColor;
		static Vec4 s_EditorSoundSourceMinMaxColor;

		static CApplicationComponentsDatabase s_ComponentDatabase;


		static Vec3 s_EditorSoundEngineListenerPosition;


		static inline constexpr const char* s_SoundSourceDescPosition = "";
		static inline constexpr const char* s_SoundSourceDescHeight = "";
		static inline constexpr const char* s_SoundSourceDescVelocity = "";
		static inline constexpr const char* s_SoundSourceDescConeOrient = "";
		static inline constexpr const char* s_SoundSourceDescConeSettings = "";
		static inline constexpr const char* s_SoundSourceDescOuterConeVolume = "";
		static inline constexpr const char* s_SoundSourceDescMinMaxDistance = "The \"min distance\" of a spatializer effect represents the distance from the listener at which the event instance is not attenuated,"
																			  "and the \"max distance\" represents the distance at which the event instance is attenuated to the point of being silent."
																			  "Between these extremes, the volume is ramped such that the closer the listener gets to the event instance, the louder it sounds";
	};


	enum FolderOptions
	{
		FolderOptions_NewFolder = 0,
		FolderOptions_NewFile,
		FolderOptions_Delete,
	};
	static const char* s_EditorAssetPanelFolderOptions[] = {
		"New Folder",
		"New File",
		"Delete"
	};

	enum FileType
	{
		FileType_Texture,
		FileType_Shader,
		FileType_Behavior,
		FileType_Script,



		FileType_Lua
	};
	static const char* s_EditorAssetPanelFileTypes[] = {
		"Texture",
		"Shader",
		"Behavior",
		"Script",
		"Scene",
		"MAML",


		"Lua"
	};
	static const char* s_EditorAssetPanelFileTypeExtensions[] = {
		".texture",
		".shader",
		".vsh",
		".fsh",
		".behavior",
		".script",
		".scene",
		".maml",


		".lua"
	};
	static const char* s_EditorDefaultFileSourceFileIcon[] = {
		ICON_FA_PANORAMA,
		ICON_FA_FILE_LINES,
		ICON_FA_FILE_CODE,
		ICON_FA_FILE_CODE,
		ICON_FA_PEOPLE_ROBBERY,
		ICON_FA_FILE_CODE,
		ICON_FA_FILE_LINES,
		ICON_FA_FILE,


		ICON_FA_FILE_CODE,
	};

	static const char* s_EditorEntityOptions[] = {
		"Create Dynamic Child",
		"Create Dynamic Parent",
		"Create Static Child",
		"Create Static Parent",
		"Delete"
	};

	static const char* s_EditorEasingOptions[] = {
			"Linear",
			"Step",
			"SmoothStep",
			"InQuad",
			"OutQuad",
			"InOutQuad",
			"OutInQuad",
			"InCubic",
			"OutCubic",
			"InOutCubic",
			"OutInCubic",
			"InQuart",
			"OutQuart",
			"InOutQuart",
			"OutInQuart",
			"InQuint",
			"OutQuint",
			"InOutQuint",
			"OutInQuint",
			"InSine",
			"OutSine",
			"InOutSine",
			"OutInSine",
			"InExpo",
			"OutExpo",
			"InOutExpo",
			"OutInExpo",
			"InCirc",
			"OutCirc",
			"InOutCirc",
			"OutInCirc",
			"InElastic",
			"OutElastic",
			"InOutElastic",
			"OutInElastic",
			"InBack",
			"OutBack",
			"InOutBack",
			"OutInBack",
			"InBounce",
			"OutBounce",
			"InOutBounce",
			"OutInBounce",
	};


	static const char* s_EditorParticleEmitterModes[] = {
		"None",
		"Gravity",
		"Free"
	};

}


#endif
#endif