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
	entt::entity GlobalData::s_EditorInspectedEntity = entt::null;
	entt::entity GlobalData::s_EditorOptionSelectedEntity = entt::null;
	entt::entity GlobalData::s_EditorOptionEntityToBeDeleted = entt::null;
	ImGuiID GlobalData::s_EditorEditFieldSliderIdStart = 10000;
	ImGuiID GlobalData::s_EditorEditFieldScalarIdStart = 50000;


	u64 GlobalData::s_EditorTransformMinPosition = -100000 * SCAST(u64, GlobalData::s_DefaultGridcellSize);
	u64 GlobalData::s_EditorTransformMaxPosition = 100000 * SCAST(u64, GlobalData::s_DefaultGridcellSize);
	f32 GlobalData::s_EditorTransformMinScale = 0.01f;
	f32 GlobalData::s_EditorTransformMaxScale = 1000.0f;
	f32 GlobalData::s_EditorTransformMinRotation = -mint::math::pi;
	f32 GlobalData::s_EditorTransformMaxRotation = mint::math::pi;

	mint::editor::CApplicationComponentsDatabase GlobalData::s_ComponentDatabase;


}
#endif