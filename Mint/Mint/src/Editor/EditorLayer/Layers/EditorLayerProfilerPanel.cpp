#if MINT_DISTR
#else
#include "EditorLayerProfilerPanel.h"


namespace mint::editor
{

	bool CProfilerPanelLayer::on_initialize()
	{
		return true;
	}


	void CProfilerPanelLayer::on_update(f32 dt)
	{
	}


	void CProfilerPanelLayer::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("CProfilerPanelLayer::on_ui_frame", "Editor::Layer");

		if (GlobalData::Get().s_EditorProfilerEnabled)
		{
			GlobalData::Get().s_EditorProfilerEnabled = m_enabled;

			ImGui::SetNextWindowSize({ percent(get_window_width(), 25), percent(get_window_height(), 25) }, ImGuiCond_Once);
			ImGui::SetNextWindowPos({ percent(get_window_width(), 50), percent(get_window_height(), 50) }, ImGuiCond_Appearing);
			ImGui::Begin("Profiler Panel", &m_enabled, get_flags());


			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Categories"))
				{
					if (ImGui::MenuItem("All"))
					{
						GlobalData::Get().s_EditorProfilerCategory = "All";
					}

					auto categories = profiler::CFunctionProfiler::Get().get_all_categories();

					for (auto& category : categories)
					{
						if (ImGui::MenuItem(category.second.c_str()))
						{
							GlobalData::Get().s_EditorProfilerCategory = category.second;
							break;
						}
					}

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Sorting"))
				{
					ImGui::Checkbox("Sort by execution time", &m_sortByMeantime);
					
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Update Interval"))
				{
					m_updateInterval = profiler::CFunctionProfiler::Get().get_update_interval_in_seconds();

					CUI::edit_field_uint32(m_updateInterval, 0, 60, "Update Interval in seconds", "", 10000, 20000, ImGuiSliderFlags_Logarithmic);

					profiler::CFunctionProfiler::Get().set_update_interval_in_seconds(m_updateInterval);

					ImGui::EndMenu();
				}
			}
			ImGui::EndMenuBar();


			main_frame();

			ImGui::End();

			GlobalData::Get().s_EditorProfilerEnabled = m_enabled;
		}
	}


	mint::String CProfilerPanelLayer::get_layer_name()
	{
		return "CProfilerPanelLayer";
	}


	void CProfilerPanelLayer::on_frame()
	{
	}


	void CProfilerPanelLayer::main_frame()
	{
		MINT_PROFILE_SCOPE("CProfilerPanelLayer::main_frame", "Editor::Layer");

		if (!profiler::CFunctionProfiler::Get().does_category_exist(GlobalData::Get().s_EditorProfilerCategory) &&
			GlobalData::Get().s_EditorProfilerCategory != "All")
		{
			ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), TextFormat("Category \"%s\" does not exist! Please select a valid one.", GlobalData::Get().s_EditorProfilerCategory.c_str()));
			return;
		}

		Vector< profiler::SFunction > stats;

		if (GlobalData::Get().s_EditorProfilerCategory == "All")
		{
			if (m_sortByMeantime)
			{
				stats = profiler::CFunctionProfiler::Get().get_all_stats_sorted_by_meantime();
			}
			else
			{
				stats = profiler::CFunctionProfiler::Get().get_all_stats_sorted_by_callcount();
			}
		}
		else
		{
			if (m_sortByMeantime)
			{
				stats = profiler::CFunctionProfiler::Get().get_stats_for_category_sorted_by_meantime(GlobalData::Get().s_EditorProfilerCategory);
			}
			else
			{
				stats = profiler::CFunctionProfiler::Get().get_stats_for_category_sorted_by_callcount(GlobalData::Get().s_EditorProfilerCategory);
			}
		}


		Vec4 color;
		Vec4 red = { 1.0f, 0.0f, 0.0f, 1.0f };
		Vec4 green = { 0.0f, 1.0f, 0.0f, 1.0f };

		if (ImGui::BeginTable("FunctionStatsTable", 4, ImGuiTableFlags_Resizable))
		{
			// Columns setup.
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Thread");
			ImGui::TableSetupColumn("Callcount");
			ImGui::TableSetupColumn("Mean execution time");
			ImGui::TableHeadersRow();

			// Table drawing.
			for (u32 row = 0; row < stats.size(); row++)
			{
				color = glm::lerp< Vec4 >(red, green, { (f32)row / stats.size(), (f32)row / stats.size(), (f32)row / stats.size(), 1.0f });

				auto& function = stats[row];

				ImGui::TableNextRow();

				ImGui::PushStyleColor(ImGuiCol_Text, { color.r, color.g, color.b, color.a });

				// Draw Name.
				u32 column = 0;
				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%s", function.m_name.c_str());


				// Draw Thread.
				ImGui::TableSetColumnIndex(column + 1);
				ImGui::Text("%zu", function.m_threadId);


				// Draw Callcount.
				ImGui::TableSetColumnIndex(column + 2);
				ImGui::Text("%zu", function.m_callcount);


				// Draw execution time.
				ImGui::TableSetColumnIndex(column + 3);
				ImGui::Text("%.5f ms", function.get_mean_execution_time());

				ImGui::PopStyleColor();
			}

			ImGui::EndTable();
		}
	}


	ImGuiWindowFlags CProfilerPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_MenuBar;
	}

}
#endif