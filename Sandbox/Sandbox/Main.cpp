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

		engine.ui_frame_begin();

		MINT_ACTIVE_SCENE()->on_after_frame(CTimestep::get_fps());

		engine.ui_frame_end();

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
	using namespace mint;

	// Entity controls.
	auto& registry = get_registry();
	auto& transform = registry.get_component< mint::component::STransform >(m_knight);
	auto& sprite = registry.get_component< mint::component::SSprite >(m_knight);

	ImGuiID sliderId = 10000, scalarId = 90000;

	ImGui::Begin("Knight");
	CUI::edit_field_vec2(transform.m_scale, 0.1f, 10.0f, "Scale", "", sliderId++, scalarId++, ImGuiSliderFlags_Logarithmic);
	CUI::edit_field_vec2(transform.m_position, -1.0f, 1.0f, "Position", "", sliderId++, scalarId++, ImGuiSliderFlags_Logarithmic);
	CUI::edit_field_f32(transform.m_rotation, 0.0f, 360.0f, "Rotation", "", sliderId++, scalarId++, ImGuiSliderFlags_Logarithmic);
	CUI::edit_field_color(sprite.m_color, 0, 255, "Color", "", sliderId++, scalarId++);
	ImGui::End();

	// Camera controls.
	auto& camera = get_active_camera();

	ImGui::Begin("Camera");
	CUI::edit_field_vec3(camera.m_transform, -500.0f, 500.0f, "Position", "", sliderId++, scalarId++, ImGuiSliderFlags_Logarithmic);
	CUI::edit_field_vec3(camera.m_lookAt, -500.0f, 500.0f, "LookAt", "", sliderId++, scalarId++, ImGuiSliderFlags_Logarithmic);
	CUI::edit_field_f32(camera.m_rotation, 0.0f, 360.0f, "Rotation", "", sliderId++, scalarId++, ImGuiSliderFlags_Logarithmic);
	CUI::edit_field_f32(camera.m_fov, 0.0f, 360.0f, "FOV", "", sliderId++, scalarId++, ImGuiSliderFlags_Logarithmic);
	ImGui::End();

	camera.recalculate_view();
	camera.recalculate_projection();
}


bool CMainScene::on_before_load()
{
	return true;
}


bool CMainScene::on_load()
{
	mint::CCamera camera(ENGINE()->get_main_viewport());
	camera.set_position({ 0.0f, 0.0f, -10.0f });
	camera.set_rotation(0.0f);

	camera.recalculate_view();
	camera.recalculate_projection();

	push_camera(camera);



	// Manually create a testing entity.
	auto& registry = get_registry();
	mint::CRandom random;

	m_knight = registry.create_entity();

	auto& transform = registry.add_component< mint::component::STransform >(m_knight);
	auto& sprite = registry.add_component< mint::component::SSprite >(m_knight);

	transform.m_position = { 0.5f, 0.5f };
	transform.m_rotation = 0.0f;
	transform.m_scale = { 1.0f, 1.0f };

	sprite.m_color = { 255, 255, 255, 255 };
	sprite.m_uv = { 1.0f, 1.0f };
	sprite.m_textureHandle = mint::CTextureManager::Get().get_texture_handle("Knight");


	add_entity(m_knight);


	m_ready = true;
	return true;
}


void CMainScene::on_before_unload()
{

}


void CMainScene::on_unload()
{

}


