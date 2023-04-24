#include "Main.h"


#ifdef MINT_PLATFORM_WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

void algorithm()
{
	using namespace mint;

	MINT_REGISTER_SCENE(CMainScene);

	CMintEngine engine;

	if (!engine.initialize("Manifest.manifest"))
	{
		return;
	}

	// Check registered services after Init.
	IService::print_registered_services();

	engine.set_engine_fps(30.0f);

	// Simulate a "Run".
	while (engine.is_running())
	{
		engine.on_before_update();

		engine.on_update(engine.get_engine_frametime());


		engine.begin_rendering();



		engine.begin_frame();

		engine.frame();

		engine.end_frame();



		engine.ui_frame_begin();

		engine.ui_frame_render();

		engine.ui_frame_end();



		engine.end_rendering();



		engine.on_after_update(engine.get_engine_frametime());
	}

	// Check registered services before Shutdown.
	IService::print_registered_services();


	engine.terminate();
}


int main(int argc, char* argv[])
{
	algorithm();

#ifdef MINT_PLATFORM_WINDOWS
	return _CrtDumpMemoryLeaks();
#else
	return 0;
#endif
}


void CMainScene::on_update(mint::f32 dt /*= 0.0f*/)
{

}


void CMainScene::on_ui_render(mint::f32 dt /*= 0.0f*/)
{
	using namespace mint;

	ImGui::Begin("Debug");
	ImGui::Text("Engine FPS: %.5f ms", MINT_ENGINE()->get_engine_fps());
	ImGui::Text("Engine Frametime: %.5f ms", MINT_ENGINE()->get_engine_frametime());
	ImGui::Text("Real FPS: %.5f ms", CTimestep::get_real_fps());
	ImGui::Text("Real Frametime: %.5f ms", CTimestep::get_real_frametime());
 	if(ImGui::Button("Quit"))
	{
		MINT_ENGINE()->exit();
	}
	ImGui::End();

}


bool CMainScene::on_before_load()
{
	return true;
}


bool CMainScene::on_load()
{
	using namespace mint;

	const auto& window = MINT_ENGINE()->get_main_window_const();

	auto camera = new mint::fx::CCamera2D({ 150, 150, 0, 255 }, 
										   window.get_x(), window.get_y(), window.get_w(), window.get_h());
	camera->set_translation({ 0.0f, 0.0f });
	camera->set_rotation(0.0f);
	camera->set_zoom(1.0f);

	push_camera(camera);


	// Create an entity.
	m_knight = m_registry.create_entity();

	auto& identifier = m_registry.add_component< mint::component::SIdentifier >(m_knight);
	auto& hierarchy = m_registry.add_component< mint::component::SSceneHierarchy >(m_knight);
	auto& transform = m_registry.add_component< mint::component::STransform >(m_knight);
	auto& sprite = m_registry.add_component< mint::component::SSprite >(m_knight);

	identifier.m_enttId = SCAST(u64, m_knight);
	identifier.m_uuid = identifier.m_enttId;
	hierarchy.m_parent = entt::null;
	transform.m_scale = { 1.0f, 1.0f };
	transform.m_rotation = 45.0f;
	transform.m_scale = { 1.0f, 1.0f };
	sprite.m_visible = true;
	sprite.m_internalVisible = true;
	sprite.m_depth = 0;
	sprite.m_rect = { 0.0f, 0.0f, 1.0f, 1.0f };
	sprite.m_color = { 255, 255, 255, 255 };
	sprite.m_origin = { 0.0f, 0.0f };


	mint::fx::CMaterial material;
	
	// Set material data and bind static uniforms once.
	material.set_shader_program("ShaderName");
	material.set_texture(0, { 180, 180 });
	material.restore_default_blend_mode();
	material.bind_static_uniforms();


	// Add material for entity.
	mint::fx::CMaterialManager::Get().add_material_for_entity(m_knight, material);

	m_ready = true;
	return true;
}


void CMainScene::on_before_unload()
{

}


void CMainScene::on_unload()
{

}


