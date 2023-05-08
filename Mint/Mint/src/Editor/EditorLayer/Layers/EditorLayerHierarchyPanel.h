#if MINT_DISTR
#else
#ifndef _MINTEDITOR_EDITOR_HIERARCHY_PANEL_LAYER_H_
#define _MINTEDITOR_EDITOR_HIERARCHY_PANEL_LAYER_H_


#include "../Common/Layer.h"
#include "SceneSystem/Common/IScene.h"


namespace mint::editor
{

	class CHierarchyPanelLayer : public CLayer
	{
	public:
		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		ImGuiWindowFlags get_flags() override final;

		String get_layer_name() override final;


	private:
		mint::String m_currentSceneName;


	private:
		void show_main_frame();

		void show_entity_recursive(entt::entity entity);

		void check_entity_for_components_sanity(entt::entity entity);
	};

}

#endif
#endif