#include "Window.h"


namespace mint
{


	CWindow::~CWindow()
	{
		terminate();
	}


	bool CWindow::initialize(SDescription& desc)
	{
		u32 flags = 0;

		if (desc.m_fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
		if (!desc.m_decorated) flags |= SDL_WINDOW_BORDERLESS;
		if (!desc.m_visible) flags |= SDL_WINDOW_HIDDEN;
		if (desc.m_minimized) flags |= SDL_WINDOW_MINIMIZED;
		if (desc.m_resizable) flags |= SDL_WINDOW_RESIZABLE;
		if (desc.m_focus) flags |= SDL_WINDOW_INPUT_FOCUS;

		m_window = SDL_CreateWindow(desc.m_title.c_str(), desc.m_width, desc.m_height, flags);


		if(m_window)
		{
			return true;
		}

		return false;
	}


	void CWindow::terminate()
	{
		if(m_window)
		{
			SDL_DestroyWindow(m_window);

			m_window = nullptr;
		}
	}


	void CWindow::show()
	{
		SDL_ShowWindow(m_window);
	}


	void CWindow::hide()
	{
		SDL_HideWindow(m_window);
	}


	mint::Vec2 CWindow::get_size()
	{
		int w, h;
		SDL_GetWindowSize(m_window, &w, &h);

		return { w, h };
	}


	void CWindow::set_size(Vec2 v)
	{
		SDL_SetWindowSize(m_window, v.x, v.y);
	}


}