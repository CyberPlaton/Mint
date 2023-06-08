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
		if (GlobalData::Get().s_EditorProfilerEnabled)
		{
			ImGui::SetNextWindowSize({ percent(get_window_width(), 25), percent(get_window_height(), 25) }, ImGuiCond_Once);
			ImGui::SetNextWindowPos({ percent(get_window_width(), 50), percent(get_window_height(), 50) }, ImGuiCond_Appearing);
			ImGui::Begin("Profiler Panel", &m_enabled, get_flags());

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
		MINT_PROFILE_SCOPE_CAT("Engine", "CProfilerPanelLayer::on_frame");
	}


	void CProfilerPanelLayer::main_frame()
	{
		auto stats = profiler::CFunctionProfiler::Get().get_stats_for_category_raw("Engine");

		for (auto& function : stats)
		{
			ImGui::Text("%s (%zu)\t%zu\t%.5f ms", function.m_name.c_str(), function.m_threadId, function.m_callcount, function.get_mean_execution_time());
		}
	}


	ImGuiWindowFlags CProfilerPanelLayer::get_flags()
	{
		return ImGuiWindowFlags_None;
	}

}
#endif