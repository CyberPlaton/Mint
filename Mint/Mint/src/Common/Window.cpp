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

		m_window.Init(desc.m_width, desc.m_height, desc.m_title);
		
		if(!desc.m_show) hide();

		m_windowIcon.Load(desc.m_window_icon_path);

		m_window.SetIcon(m_windowIcon)
				.SetTargetFPS(desc.m_targetFPS);

		_setWindowState(FLAG_WINDOW_UNDECORATED, !desc.m_decorated);
		_setWindowState(FLAG_WINDOW_MINIMIZED, desc.m_minimized);
		_setWindowState(FLAG_WINDOW_RESIZABLE, desc.m_resizable);
		_setWindowState(FLAG_WINDOW_ALWAYS_RUN, desc.m_runMinimized);

		if (desc.m_fullscreen)
		{
			m_window.SetFullscreen(true);

			_setWindowState(FLAG_WINDOW_MAXIMIZED, true);

			_setWindowState(FLAG_WINDOW_TOPMOST, true);

			_setWindowState(FLAG_WINDOW_UNFOCUSED, false);
		}


		if (desc.m_show) show();

		return true;
	}


	void CWindow::terminate()
	{

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
		auto size = m_window.GetSize();
		return { size.x, size.y };
	}


	void CWindow::set_size(Vec2 v)
	{
		m_window.SetSize({ v.x, v.y });
	}


	void CWindow::toggle_fullscreen()
	{
		bool v = !m_window.IsFullscreen();

		m_window.SetFullscreen(v);
	}


	void CWindow::set_title(const String& title)
	{
		m_window.SetTitle(title);
	}


	mint::f32 CWindow::get_x() const
	{
		return m_window.GetPosition().x;
	}


	mint::f32 CWindow::get_y() const
	{
		return m_window.GetPosition().y;
	}


	mint::f32 CWindow::get_w() const
	{
		return m_window.GetSize().x;
	}


	mint::f32 CWindow::get_h() const
	{
		return m_window.GetSize().y;
	}


	void CWindow::_setWindowState(ConfigFlags flag, bool value)
	{
		value == true ? SetWindowState(flag) : ClearWindowState(flag);
	}


}