#if MINT_DISTR
#else
#include "EditorCommon.h"


namespace mint::editor
{
	mint::editor::GlobalData* GlobalData::s_GlobalData = nullptr;
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
	const mint::u64 GlobalData::s_DefaultEditorTextEditorBufferSize = 1024 * 13.0f;
	entt::entity GlobalData::s_EditorInspectedEntity = entt::null;
	ImGuiID GlobalData::s_EditorEditFieldSliderIdStart = 10000;
	ImGuiID GlobalData::s_EditorEditFieldScalarIdStart = 50000;


}
#endif