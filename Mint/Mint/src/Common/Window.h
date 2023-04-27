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
			/// @brief Title of the window.
			String m_title;

			/// @brief Path too the window icon. Loaded directly and managed by window. App fail if bad string.
			String m_window_icon_path;

			/// @brief Width of the window.
			u64 m_width;

			/// @brief Height of the window.
			u64 m_height;

			/// @brief The target Frames-Per-Second. If we are above the CPU is set to rest saving energy.
			u64 m_targetFPS;

			/// @brief Whether to run in fullscreen.
			bool m_fullscreen;

			/// @brief Try to enable VSync for GPU.
			bool m_vsync;

			/// @brief Whether to allow the window to be resizable.
			bool m_resizable;

			/// @brief Whether to add top bar for window with icon and close button.
			bool m_decorated;

			/// @brief Whether to show the window.
			bool m_show;

			/// @brief Whether the window should take focus of mouse and keyboard.
			bool m_focus;

			/// @brief Whether to minimize window and leave as icon on task-bar.
			bool m_minimized;

			/// @brief Whether the program should continue running though minimized.
			bool m_runMinimized;
		};

	public:
		CWindow() = default;
		~CWindow();


		bool initialize(SDescription& desc);

		void terminate();

		void show();

		void hide();

		Vec2 get_size();

		void set_size(Vec2 v);

		void toggle_fullscreen();

		void set_title(const String& title);

		f32 get_x() const;

		f32 get_y() const;

		f32 get_w() const;

		f32 get_h() const;


	private:
		Window m_window;
		Texture m_windowIcon;
		

	private:
		void _setWindowState(ConfigFlags flag, bool value);
	};


}


#endif