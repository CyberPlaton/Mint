#if MINT_DISTR
#else
#ifndef _MINTEDITOR_EDITOR_VIEWPORT_PANEL_LAYER_H_
#define _MINTEDITOR_EDITOR_VIEWPORT_PANEL_LAYER_H_


#include "../Common/Layer.h"
#include "../../RessourceManagement/EditorIconManager.h"
#include "Graphics/RenderingPasses/DebugRenderer/DebugRenderer.h"
#include "WorldQuery/WorldQueryDebugRenderer.h"
#include "SoundSystem/SoundSystemDebugRenderer.h"
#include "SceneSystem/SAS.h"
#include "Graphics/RenderingPassStack.h"
#include "Graphics/RenderingPasses/AntiAliasing/FXAA.h"
#include "Utility/Profiling/FunctionProfiler.h"
#include "SoundSystem/SoundSystem.h"


namespace mint::editor
{

	class CViewportPanelLayer : public CLayer
	{
	public:
		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		void on_frame() override final;

		String get_layer_name() override final;

		ImGuiWindowFlags CViewportPanelLayer::get_flags() override final;

	private:

	private:
		void main_frame();

		void window_options();
		
		void camera_options();

		void grid_options();

		void debug_render_options();

		void sound_engine_options();
	};

}

#endif
#endif