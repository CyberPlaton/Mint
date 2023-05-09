#if MINT_DISTR
#else
#ifndef _MINTEDITOR_EDITOR_COMMON_H_
#define _MINTEDITOR_EDITOR_COMMON_H_



#include "UI/UI.h"
#include "../../Editor/ComponentEditor/ApplicationComponentsDatabase.h"


namespace mint::editor
{
	struct GlobalData
	{
		STATIC_GET(GlobalData, s_GlobalData);

		static GlobalData* s_GlobalData;
		
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
		static const mint::u64 s_DefaultEditorTextEditorBufferSize;
		static entt::entity s_EditorInspectedEntity;


		static ImGuiID s_EditorEditFieldSliderIdStart;
		static ImGuiID s_EditorEditFieldScalarIdStart;


		static u64 s_EditorTransformMinPosition;
		static u64 s_EditorTransformMaxPosition;
		static f32 s_EditorTransformMinScale;
		static f32 s_EditorTransformMaxScale;
		static f32 s_EditorTransformMinRotation;
		static f32 s_EditorTransformMaxRotation;

		static CApplicationComponentsDatabase s_ComponentDatabase;
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

}


#endif
#endif