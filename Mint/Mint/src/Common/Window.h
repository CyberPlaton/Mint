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

		void show();

		void hide();

		Vec2 get_size();

		void set_size(Vec2 v);



	private:
		Vec4 m_clearColor;

	};


}


#endif