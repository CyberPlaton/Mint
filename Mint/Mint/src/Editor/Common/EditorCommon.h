#ifndef _MINTEDITOR_EDITOR_COMMON_H_
#define _MINTEDITOR_EDITOR_COMMON_H_


#include "UI/UI.h"


namespace mint::editor
{

	static const mint::String s_EditorIconsPath = "EditorRessources/Icons";
	static mint::String s_EditorDefaultSceneRessourcesPath = "assets";
	static const mint::u32 s_DefaultIconSize = 16;
	static const mint::f32 s_DefaultGridcellSize = 64.0f;
	static mint::f32 s_DefaultEditorCameraSpeed = 200.0f;
	static mint::f32 s_DefaultEditorCameraSpeedDecreased = 20.0f;
	static mint::f32 s_DefaultEditorCameraZoomSpeed = 0.5f;
	static mint::f32 s_DefaultEditorCameraZoomSpeedDecreased = 0.05f;
}


#endif