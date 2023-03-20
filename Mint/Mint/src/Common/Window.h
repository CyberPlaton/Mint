#ifndef _MINT_WINDOW_H_
#define _MINT_WINDOW_H_


#include "Common.h"
#include "Algorithm.h"


namespace mint
{

	class CWindow
	{
	public:
		struct SDescription
		{
			String m_title;

			u64 m_width;
			u64 m_height;

			u64 m_targetFPS;

			bool m_fullscreen;

			bool m_resizable;

			bool m_decorated;

			bool m_visible;

			bool m_minimized;

			bool m_focus;

			bool m_vsync;

			Vec4 m_clearColor;
		};

	public:
		CWindow() = default;
		~CWindow();


		bool initialize(SDescription& desc);

		void terminate();


		template< typename T >
		T get_native_handle();


		void show();

		void hide();

		Vec2 get_size();

		void set_size(Vec2 v);



	private:
		Window* m_window;

		Vec4 m_clearColor;

		bgfx::ViewId m_viewId;
	};


	template< typename T >
	T mint::CWindow::get_native_handle()
	{
		SDL_SysWMinfo info;
		SDL_GetWindowWMInfo(m_window, &info, SDL_SYSWM_CURRENT_VERSION);

#ifdef MINT_PLATFORM_WINDOWS
		return reinterpret_cast< T >(info.info.win.window);
#endif
#ifdef MINT_PLATFORM_LINUX
		return reinterpret_cast< T >(info.info.x11.window);
#endif


	}


}


#endif