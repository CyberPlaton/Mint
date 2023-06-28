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
	bool GlobalData::s_EditorProfilerEnabled = false;
	mint::String GlobalData::s_EditorProfilerCategory = "All";


	bool GlobalData::s_EditorBehaviorScriptsEnabled = true;
	bool GlobalData::s_EditorScriptScriptsEnabled = true;
	bool GlobalData::s_EditorAnimationsEnabled = true;


	mint::Vec4 GlobalData::s_EditorWorldQueryAABBColor = {150, 0, 0, 30};

	bool GlobalData::s_EditorDebugRenderAABBFullInformation = false;
	bool GlobalData::s_EditorDebugRenderAABBs = false;
	bool GlobalData::s_EditorDebugRenderOriginPoint = false;
	bool GlobalData::s_EditorDebugRenderDestinationRect = false;
	bool GlobalData::s_EditorDebugRenderSelected = false;
	bool GlobalData::s_EditorDebugRenderAll = false;

}
#endif