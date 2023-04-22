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


		engine.begin_rendering();

		engine.begin_frame();

		engine.frame();

		engine.end_frame();


		
		engine.ui_frame_begin();

		MINT_ACTIVE_SCENE()->on_after_frame(CTimestep::get_fps());

		engine.ui_frame_end();

		engine.end_rendering();

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
	const auto& window = MINT_ENGINE()->get_main_window_const();

	auto camera = new mint::fx::CCamera2D({ 150, 150, 0, 255 }, 
										   window.get_x(), window.get_y(), window.get_w(), window.get_h());
	camera->set_translation({ 0.0f, 0.0f });
	camera->set_rotation(0.0f);
	camera->set_zoom(1.0f);

	push_camera(camera);

	m_ready = true;
	return true;
}


void CMainScene::on_before_unload()
{

}


void CMainScene::on_unload()
{

}


