#include "Mint.h"

namespace mint
{
	
	bool CMintEngine::initialize(const String& manifest_filepath)
	{
		_prepare_for_init();

		CScene* initial_scene = nullptr;
		CWindow::SDescription desc;
		maml::SNode* maml_root = nullptr;



		return true;
	}


	void CMintEngine::terminate()
	{
		
	}


	void CMintEngine::reset()
	{

	}


	bool CMintEngine::is_running()
	{
		return m_running;
	}


	void CMintEngine::exit()
	{
		m_running = false;
	}


	void CMintEngine::on_update(f32 dt)
	{
		_on_update(dt);
	}


	void CMintEngine::begin_frame()
	{
		_begin_frame();
	}


	void CMintEngine::frame()
	{
		_frame();
	}


	void CMintEngine::end_frame()
	{
		_end_frame();
	}


	void CMintEngine::on_before_update()
	{
		_on_pre_update();
	}


	void CMintEngine::on_after_update(f32 dt)
	{
		_on_post_update();
	}


	bool CMintEngine::_prepare_for_init()
	{

	}


	void CMintEngine::_on_pre_update()
	{

	}


	void CMintEngine::_on_update(f32 dt /*= CTimestep::get_fps()*/)
	{

	}


	void CMintEngine::_on_post_update()
	{

	}


	void CMintEngine::_on_late_update()
	{

	}


	void CMintEngine::_begin_frame()
	{

	}


	void CMintEngine::_frame()
	{

	}


	void CMintEngine::_end_frame()
	{

	}


}