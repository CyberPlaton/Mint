#include "Main.h"

int main(int argc, char* argv[])
{
	using namespace mint;

	MINT_REGISTER_SCENE(CMainScene);

	CMintEngine engine;

	if(!engine.initialize("Manifest.manifest"))
	{
		return -1;
	}

	// Simulate a "Run".
	while(engine.is_running())
	{
		engine.on_before_update();

		engine.on_update(CTimestep::get_fps());


		engine.begin_frame();

		engine.frame();

		engine.end_frame();


		
		engine.ui_frame_begin();

		MINT_ACTIVE_SCENE()->on_after_frame(CTimestep::get_fps());

		engine.ui_frame_end();

		engine.on_after_update(CTimestep::get_fps());
	}


	engine.terminate();
}


void CMainScene::on_update(mint::f32 dt /*= 0.0f*/)
{

}


void CMainScene::on_after_frame(mint::f32 dt /*= 0.0f*/)
{
	using namespace mint;

	ImGui::Begin("Debug");
	ImGui::Text("FPS: %.5f ms", CTimestep::get_fps());
	ImGui::Text("Frametime: %.5f ms", CTimestep::get_frametime());
	ImGui::End();
}


bool CMainScene::on_before_load()
{
	return true;
}


bool CMainScene::on_load()
{
	
	m_ready = true;
	return true;
}


void CMainScene::on_before_unload()
{

}


void CMainScene::on_unload()
{

}


