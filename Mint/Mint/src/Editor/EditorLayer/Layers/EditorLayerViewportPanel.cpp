#if MINT_DISTR
#else
#include "EditorLayerViewportPanel.h"


namespace mint::editor
{

	bool CViewportPanelLayer::on_initialize()
	{
		return true;
	}


	void CViewportPanelLayer::on_update(f32 dt)
	{

	}


	void CViewportPanelLayer::on_ui_frame()
	{
		ImGui::SetNextWindowSize({ percent(get_window_width(), 50), percent(get_window_height(), 5.5f) }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ percent(get_window_width(), 25), percent(get_window_height(), 2.0f) }, ImGuiCond_Appearing);
		ImGui::Begin("Viewport Panel", &m_enabled, get_flags());


		ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.9f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

		ImGui::SameLine();

		if(ImGui::SmallButton(ICON_FA_DISPLAY)) { ImGui::OpenPopup("Viewport Window Options Popup"); }
		CUI::help_marker_no_question_mark("Edit window settings");

		window_options();

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_VIDEO)) { ImGui::OpenPopup("Viewport Camera Options Popup"); }
		CUI::help_marker_no_question_mark("Edit camera settings");

		camera_options();

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_TABLE_CELLS)) { ImGui::OpenPopup("Viewport Grid Options Popup"); }
		CUI::help_marker_no_question_mark("Edit grid settings");

		grid_options();

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_BUG)) { ImGui::OpenPopup("Viewport Debug Render Options Popup"); }
		CUI::help_marker_no_question_mark("Edit debug rendering settings");

		debug_render_options();

		ImGui::PopStyleColor(2);

		main_frame();

		ImGui::End();
	}


	mint::String CViewportPanelLayer::get_layer_name()
	{
		return "CViewportPanelLayer";
	}


	void CViewportPanelLayer::on_frame()
	{
		if(MINT_ENGINE()->is_in_editor_mode())
		{
			auto camera = MINT_ACTIVE_SCENE()->get_active_camera();

			fx::CDebugRenderer::begin(camera);

			if(m_debugRenderAll)
			{
				auto& entities = CSAS::Get().retrieve_visible_entities();
				
				if(m_renderDestinationRect) fx::CDebugRenderer::on_render_destination_rectangle(entities);

				if(m_renderOriginPoint) fx::CDebugRenderer::on_render_sprite_origin(entities);
			}
			else
			{
				if(GlobalData::Get().s_EditorInspectedEntity != entt::null)
				{
					Vector< entt::entity > entities{ GlobalData::Get().s_EditorInspectedEntity };

					if(m_renderDestinationRect && MINT_ACTIVE_SCENE()->get_registry().has_component< mint::component::STransform >(GlobalData::Get().s_EditorInspectedEntity))
					{
						fx::CDebugRenderer::on_render_destination_rectangle(entities);
					}

					if (m_renderOriginPoint && MINT_ACTIVE_SCENE()->get_registry().has_component< mint::component::SSprite >(GlobalData::Get().s_EditorInspectedEntity))
					{
						fx::CDebugRenderer::on_render_sprite_origin(entities);
					}
				}
			}

			fx::CDebugRenderer::end();
		}
	}


	void CViewportPanelLayer::main_frame()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Viewport"))
			{
				if (ImGui::MenuItem("Item"))
				{
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}


	ImGuiWindowFlags CViewportPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
			 | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
			 | ImGuiWindowFlags_NoScrollbar;
	}


	void CViewportPanelLayer::window_options()
	{
		if (ImGui::BeginPopup("Viewport Window Options Popup"))
		{
			ImGui::SeparatorText("Window Settings");
			
			ImGui::SameLine();

			CUI::help_marker("Please note that changing the window dimension is done in the application \"Manifest\"!");

			auto& window = MINT_ENGINE()->get_main_window_ref();

			auto size = window.get_size();
			auto max_size = window.get_max_possible_window_size();
			auto fullscreen = window.is_fullscreen();

			ImGui::Text(TextFormat("Size: {%.1f:%.1f}", size.x, size.y));
			ImGui::Text(TextFormat("Max Size: {%.1f:%.1f}", max_size.x, max_size.y));
			if(ImGui::Checkbox("Fullscreen", &fullscreen))
			{
				window.set_is_fullscreen(fullscreen);
			}
			
			ImGui::EndPopup();
		}
	}


	void CViewportPanelLayer::camera_options()
	{
		if (ImGui::BeginPopup("Viewport Camera Options Popup"))
		{
			ImGui::SeparatorText("Editor Camera Settings");



			ImGui::EndPopup();
		}
	}


	void CViewportPanelLayer::grid_options()
	{
		if (ImGui::BeginPopup("Viewport Grid Options Popup"))
		{
			ImGui::SeparatorText("Grid Settings");



			ImGui::EndPopup();
		}
	}


	void CViewportPanelLayer::debug_render_options()
	{
		if (ImGui::BeginPopup("Viewport Debug Render Options Popup"))
		{
			ImGui::SeparatorText("Debug Render Settings");

			ImGui::Checkbox("Render All", &m_debugRenderAll);
			m_debugRenderSelected = !m_debugRenderAll;

			ImGui::SameLine();

			CUI::help_marker("Change whether to draw debug information for all game objects or for the inspected one only!");

			ImGui::Checkbox("Destination Rectangle", &m_renderDestinationRect);
			ImGui::Checkbox("Origin Point", &m_renderOriginPoint);


			ImGui::EndPopup();
		}
	}


}
#endif