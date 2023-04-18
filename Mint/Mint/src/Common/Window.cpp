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


		return false;
	}


	void CWindow::terminate()
	{

	}





	void CWindow::show()
	{
	}


	void CWindow::hide()
	{
	}


	mint::Vec2 CWindow::get_size()
	{
		return { 0.0f, 0.0f };
	}


	void CWindow::set_size(Vec2 v)
	{
	}


}