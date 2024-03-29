#include "Window.h"


namespace mint
{


	CWindow::~CWindow()
	{
		terminate();
	}


	bool CWindow::initialize(SDescription& desc)
	{
		_setWindowState(FLAG_VSYNC_HINT, desc.m_vsync);

		InitWindow(desc.m_width, desc.m_height, desc.m_title.c_str());

		hide();


		auto icon_texture = LoadImage(desc.m_window_icon_path.c_str());
		SetWindowIcon(icon_texture);

		
		auto monitor_refresh_rate = GetMonitorRefreshRate(GetCurrentMonitor());
		if(desc.m_targetFPS > monitor_refresh_rate)
		{
			desc.m_targetFPS = monitor_refresh_rate;
		}

		SetTargetFPS(desc.m_targetFPS);
		
		_setWindowState(FLAG_WINDOW_UNDECORATED, !desc.m_decorated);
		_setWindowState(FLAG_WINDOW_MINIMIZED, desc.m_minimized);
		_setWindowState(FLAG_WINDOW_RESIZABLE, desc.m_resizable);
		_setWindowState(FLAG_WINDOW_ALWAYS_RUN, desc.m_runMinimized);

		if (desc.m_fullscreen)
		{
			auto w = GetMonitorWidth(GetCurrentMonitor());
			auto h = GetMonitorHeight(GetCurrentMonitor());

			SetWindowPosition(0, 0);
			SetWindowSize(w, h);

			_setWindowState(FLAG_WINDOW_MAXIMIZED, true);

			_setWindowState(FLAG_WINDOW_TOPMOST, true);

			_setWindowState(FLAG_FULLSCREEN_MODE, true);

			_setWindowState(FLAG_WINDOW_UNFOCUSED, false);
		}


		if (desc.m_show) show();

		m_description = desc;

		return true;
	}


	void CWindow::terminate()
	{
		CloseWindow();
	}





	void CWindow::show()
	{
		_setWindowState(FLAG_WINDOW_HIDDEN, false);
	}


	void CWindow::hide()
	{
		_setWindowState(FLAG_WINDOW_HIDDEN, true);
	}


	mint::Vec2 CWindow::get_size()
	{
		return { GetScreenWidth(), GetScreenHeight() };
	}


	mint::Vec2 CWindow::get_max_possible_window_size()
	{
		auto monitor = GetCurrentMonitor();
		auto monitor_width = GetMonitorWidth(monitor);
		auto monitor_height = GetMonitorHeight(monitor);

		return { monitor_width, monitor_height };
	}


	void CWindow::set_size(const Vec2& v)
	{
		SetWindowPosition(0, 0);
		SetWindowSize((u32)v.x, (u32)v.y);
	}


	void CWindow::toggle_fullscreen()
	{
		ToggleFullscreen();
	}


	bool CWindow::is_fullscreen() const
	{
		return IsWindowFullscreen();
	}


	void CWindow::set_is_fullscreen(bool value)
	{
		if (value)
		{
			if (!is_fullscreen()) toggle_fullscreen();
		}
		else
		{
			if(is_fullscreen()) toggle_fullscreen();
		}
	}


	void CWindow::set_title(const String& title)
	{
		SetWindowTitle(title.c_str());
	}


	mint::f32 CWindow::get_x() const
	{
		return GetWindowPosition().x;
	}


	mint::f32 CWindow::get_y() const
	{
		return GetWindowPosition().y;
	}


	mint::f32 CWindow::get_w() const
	{
		return GetScreenWidth();
	}


	mint::f32 CWindow::get_h() const
	{
		return GetScreenHeight();
	}


	void CWindow::print_window_configurations() const
	{
		MINT_LOG_INFO("Dumping window configuration:", MINT_APP_TIME);

		auto monitor = GetCurrentMonitor();
		auto refresh_rate = GetMonitorRefreshRate(monitor);
		auto width = GetScreenWidth();
		auto height = GetScreenHeight();
		auto monitor_width = GetMonitorWidth(monitor);
		auto monitor_height = GetMonitorHeight(monitor);
		auto render_width = GetRenderWidth();
		auto render_height = GetRenderHeight();

		MINT_LOG_INFO("\t Monitor ID: {}", monitor);
		MINT_LOG_INFO("\t Monitor Width: {}", monitor_width);
		MINT_LOG_INFO("\t Monitor Height: {}", monitor_height);
		MINT_LOG_INFO("\t Refresh Rate: {}", refresh_rate);
		MINT_LOG_INFO("\t Target FPS: {}", m_description.m_targetFPS);
		MINT_LOG_INFO("\t Title: {}", m_description.m_title);
		MINT_LOG_INFO("\t Icon: {}", m_description.m_window_icon_path);
		MINT_LOG_INFO("\t Window Width: {}", width);
		MINT_LOG_INFO("\t Window Height: {}", height);
		MINT_LOG_INFO("\t Render Width: {}", render_width);
		MINT_LOG_INFO("\t Render Height: {}", render_height);

		MINT_LOG_INFO("\t Fullscreen: {}",		m_description.m_fullscreen == true ? "True" : "False");
		MINT_LOG_INFO("\t VSYNC: {}",			m_description.m_vsync == true ? "True" : "False");
		MINT_LOG_INFO("\t Resizable: {}",		m_description.m_resizable == true ? "True" : "False");
		MINT_LOG_INFO("\t Decorated: {}",		m_description.m_decorated == true ? "True" : "False");
		MINT_LOG_INFO("\t Show: {}",			m_description.m_show == true ? "True" : "False");
		MINT_LOG_INFO("\t Focus: {}",			m_description.m_focus == true ? "True" : "False");
		MINT_LOG_INFO("\t Minimized: {}",		m_description.m_minimized == true ? "True" : "False");
		MINT_LOG_INFO("\t Run Minimized: {}",	m_description.m_runMinimized == true ? "True" : "False");
		MINT_LOG_SEPARATOR();
	}


	void CWindow::_setWindowState(ConfigFlags flag, bool value)
	{
		value == true ? SetWindowState(flag) : ClearWindowState(flag);
	}


}