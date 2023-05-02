#ifndef _MINTEDITOR_EDITOR_COMMON_H_
#define _MINTEDITOR_EDITOR_COMMON_H_


#include "UI/UI.h"


namespace mint::editor
{

	static const mint::String s_EditorIconsPath = "EditorRessources/Icons";
	static mint::String s_EditorDefaultSceneRessourcesPath = "assets";
	static const mint::u32 s_DefaultIconSize = 20;
	static const mint::f32 s_DefaultComboWidth = 20.0f;
	static const mint::f32 s_DefaultGridcellSize = 64.0f;
	static mint::f32 s_DefaultEditorCameraSpeed = 200.0f;
	static mint::f32 s_DefaultEditorCameraSpeedDecreased = 20.0f;
	static mint::f32 s_DefaultEditorCameraZoomSpeed = 0.5f;
	static mint::f32 s_DefaultEditorCameraZoomSpeedDecreased = 0.05f;
	static const mint::f32 s_DefaultEditorDialogWidth = 500.0f;
	static const mint::f32 s_DefaultEditorDialogHeight = 200.0f;
	static const mint::f32 s_DefaultEditorTextEditorWidth = 600.0f;
	static const mint::f32 s_DefaultEditorTextEditorHeight = 400.0f;
	static const mint::f32 s_DefaultEditorCharacterSize = 13.0f;
	static const mint::u64 s_DefaultEditorTextEditorBufferSize = 1024 * 13.0f;
	static entt::entity s_EditorInspectedEntity = entt::null;


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
		FileType_VertexShader,
		FileType_FragmentShader,
		FileType_Behavior,
		FileType_Script,



		FileType_Lua
	};
	static const char* s_EditorAssetPanelFileTypes[] = {
		"Texture",
		"Vertex Shader",
		"Fragment Shader",
		"Behavior",
		"Script",
		"Scene",
		"MAML",


		"Lua"
	};
	static const char* s_EditorAssetPanelFileTypeExtensions[] = {
		"texture",
		"vsh",
		"fsh",
		"behavior",
		"script",
		"scene",
		"maml",


		"lua"
	};
	static const char* s_EditorDefaultFileSourceFileIcon[] = {
		ICON_FA_PANORAMA,
		ICON_FA_IMAGE,
		ICON_FA_IMAGE,
		ICON_FA_PEOPLE_ROBBERY,
		ICON_FA_FILE_CODE,
		ICON_FA_FILE_LINES,
		ICON_FA_FILE,


		ICON_FA_FILE_CODE,
	};

}


#endif