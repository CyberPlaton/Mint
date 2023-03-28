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

		MINT_ACTIVE_SCENE()->on_after_frame(CTimestep::get_fps());

		engine.end_frame();


		engine.on_after_update(CTimestep::get_fps());
	}


	engine.terminate();
}


void CMainScene::on_update(mint::f32 dt /*= 0.0f*/)
{

}


void CMainScene::on_after_frame(mint::f32 dt /*= 0.0f*/)
{
	ImGui::Begin("Window");
	ImGui::Text("Hello World");
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


