#ifndef _MINTEDITOR_EDITOR_PROJECT_ASSETS_PANEL_LAYER_H_
#define _MINTEDITOR_EDITOR_PROJECT_ASSETS_PANEL_LAYER_H_


#include "../Common/Layer.h"
#include "../../RessourceManagement/EditorIconManager.h"


namespace mint::editor
{

	class CProjectAssetsPanelLayer : public CLayer
	{
	public:
		bool on_initialize() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

		String get_layer_name() override final;

		ImGuiWindowFlags get_flags() override final;



	protected:

		mint::String m_currentSceneName;
		mint::String m_currentScenePathRelative;
		mint::String m_currentScenePathFull;


	protected:
		void menu_bar();

		void main_frame();

		void show_folder_contents(CPath& path);

		void show_file(const String& file_path);
	};

}


#endif