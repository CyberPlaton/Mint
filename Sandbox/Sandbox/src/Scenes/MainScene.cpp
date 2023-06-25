#include "MainScene.h"

void CMainScene::on_update(mint::f32 dt /*= 0.0f*/)
{
	CUCA::transform_rotate(m_knight,  mint::algorithm::degree_to_radians(45.0f * dt));
}


void CMainScene::on_ui_render(mint::f32 dt /*= 0.0f*/)
{
	using namespace mint;

	f32 engine_dt = MINT_ENGINE()->get_engine_fps();

	ImGui::Begin("Debug");
	ImGui::Text("Engine FPS: %.5f ms", engine_dt);
	ImGui::Text("Engine Frametime: %.5f ms", MINT_ENGINE()->get_engine_frametime());
	ImGui::Text("Real FPS: %.5f ms", CTimestep::get_real_fps());
	ImGui::Text("Real Frametime: %.5f ms", CTimestep::get_real_frametime());

	CUI::edit_field_f32(engine_dt, 1.0f, 240.0f, "Edit engine FPS", "", 1000, 1001);

	if (ImGui::Button("Quit"))
	{
		MINT_ENGINE()->exit();
	}
	ImGui::End();

	MINT_ENGINE()->set_engine_fps(engine_dt);


	// World Query testing with visualization.
// 	static mint::CRect rect = { 0.0f, 0.0f, 1.0f, 1.0f };
// 	b2AABB aabb = mint::algorithm::compute_aabb(rect);
// 	static Vec2 ray_start = { 0.0f, 0.0f };
// 	static Vec2 ray_end = {1.0f, 1.0f};
// 
// 	ImGuiID sid = 10000, scid = 20000;
// 
// 	ImGui::Begin("World Query");
// 
// 	CUI::edit_field_rect(rect, -2000.0f, 2000.0f, "Query Rectangle", "", sid++, scid++);
// 
// 	u32 result = CWorldQuery::Get().get_entity_count_at_point_in_radius(rect);
// 
// 	ImGui::Text(TextFormat("Count query AABB: %d", result));
// 
// 	TestWorldQueryFilter filter;
// 	Vector< SWorldQueryProxy > result_entities = CWorldQuery::Get().get_entities_at_point_in_radius< TestWorldQueryFilter >(rect, &filter);
// 
// 	for (auto& entity : result_entities)
// 	{
// 		ImGui::Text(TextFormat("Filter Query AABB: %zu (entt::entity)", (u64)entity.m_entity));
// 	}
// 
// 	bool any_result = CWorldQuery::Get().get_any_entity_at_point_in_radius(rect);
// 
// 	ImGui::Text(TextFormat("Any query AABB: %s", any_result == true ? "True" : "False"));
// 
// 
// 	ImGui::Separator();
// 
// 	CUI::edit_field_vec2(ray_start, -2000.0f, 2000.0f, "Query ray start", "", sid++, scid++);
// 	CUI::edit_field_vec2(ray_end, -2000.0f, 2000.0f, "Query ray end", "", sid++, scid++);
// 
// 	result = CWorldQuery::Get().get_entity_count_by_ray_intersection(ray_start, ray_end);
// 
// 	ImGui::Text(TextFormat("Count query Raycast: %d", result));
// 
// 	result_entities = CWorldQuery::Get().get_entities_by_ray_intersection< TestWorldQueryFilter >(ray_start, ray_end, &filter);
// 
// 	for (auto& entity : result_entities)
// 	{
// 		ImGui::Text(TextFormat("Filter Query Raycast: %zu (entt::entity)", (u64)entity.m_entity));
// 	}
// 
// 	any_result = CWorldQuery::Get().get_any_entity_ray_intersection(ray_start, ray_end);
// 
// 	ImGui::Text(TextFormat("Any query Raycast: %s", any_result == true ? "True" : "False"));
// 
// 	ImGui::End();
// 
// 
// 	fx::CCameraManager::Get().get_active_camera()->use_camera();
// 
// 	// Render aabb, this is the same as rendering a rect computed from aabb.
// 	fx::CPrimitiveRenderer::render_aabb_filled(aabb, { 125, 125, 125, 30 });
// 
// 	// Render rect.
// 	fx::CPrimitiveRenderer::render_rectangle_filled(rect, { 0, 255, 125, 30 });
// 
// 	// Render raycast.
// 	fx::CPrimitiveRenderer::render_line(ray_start, ray_end, MINT_RED(), 2.0f);
// 
// 	fx::CCameraManager::Get().get_active_camera()->end_camera();
}


bool CMainScene::on_before_load()
{
	const auto& window = MINT_ENGINE()->get_main_window_const();

	auto color = MINT_GREY_DARK();

	mint::fx::CCameraManager::Get().set_default_camera< mint::fx::CFreeMovementCamera >("DefaultCamera", color, 0, 0, window.get_w(), window.get_h(), 1.0f, 0.0f);


	return true;
}


bool CMainScene::on_load()
{
	using namespace mint;

	// Create an entity.
// 	m_knight = m_registry.create_entity();
// 
// 	auto& identifier = m_registry.add_component< mint::component::SIdentifier >(m_knight);
// 	auto& hierarchy = m_registry.add_component< mint::component::SSceneHierarchy >(m_knight);
// 	auto& transform = m_registry.add_component< mint::component::STransform >(m_knight);
// 	auto& sprite = m_registry.add_component< mint::component::SSprite >(m_knight);
// 	auto& script = m_registry.add_component< mint::component::SScript >(m_knight);
// 	auto& dynamic = m_registry.add_component< mint::component::SDynamicGameobject >(m_knight);
// 
// 
// 	identifier.m_enttId = SCAST(u64, m_knight);
// 	identifier.m_uuid = identifier.m_enttId;
// 	identifier.m_debugName = "Gopnik";
// 	hierarchy.m_parent = entt::null;
// 
// 	CUCA::transform_set_scale(m_knight, { 1.0f, 1.0f });
// 	CUCA::transform_set_rotation(m_knight, 0);
// 	CUCA::transform_set_position(m_knight, { 0, 0 });
// 
// 
// 	sprite.m_visible = true;
// 	sprite.m_internalVisible = true;
// 	sprite.m_depth = 1;
// 	sprite.m_rect = { 0.0f, 0.0f, 0.0f, 0.0f };
// 	sprite.m_color = { 255, 255, 255, 255 };
// 	sprite.m_origin = { 0.0f, 0.0f };
// 
// 
// 
// 	// Set script for entity.
// 	mint::scripting::CBehaviorEngine::Get().set_behavior_for_entity("SoldierController", m_knight);
// 
// 
// 	// Set animator for entity.
// 	animation::CAnimationSystem::Get().request_entity_registration(m_knight);
// 
// 	animation::CAnimationSystem::Get().set_entity_animation(m_knight, "anim_gopnik_idle");

//  	add_entity(m_knight);

 
	/*
		TESTING REFLECTION
	*/
// #if MINT_DISTR
// #else
// 	auto& reflected = m_registry.add_component< MyComponent >(m_knight);
// 	reflected.m_entity = SCAST(u64, m_knight);
// 	reflected.m_name = identifier.m_debugName;
// 	reflected.m_isReflected = true;
//  #endif





	mint::CRandom random;
	for (int i = 0; i < 150; i++)
	{
		// Create an entity.
		entt::entity entity = m_registry.create_entity();

		auto& identifier = m_registry.add_component< mint::component::SIdentifier >(entity);
		auto& hierarchy = m_registry.add_component< mint::component::SSceneHierarchy >(entity);
		auto& transform = m_registry.add_component< mint::component::STransform >(entity);
		auto& sprite = m_registry.add_component< mint::component::SSprite >(entity);
		auto& dynamic = m_registry.add_component< mint::component::SDynamicGameobject >(entity);


		identifier.m_enttId = SCAST(u64, entity);
		identifier.m_uuid = identifier.m_enttId;
		identifier.m_debugName = "Entity_" + std::to_string(SCAST(u64, entity));
		hierarchy.m_parent = entt::null;

		CUCA::transform_set_scale(entity, { 1.0f, 1.0f }); // Scaling bugged?
		CUCA::transform_set_rotation(entity, 0.0f);
 		CUCA::transform_set_position(entity, { random.normalized_float() * 1024, random.normalized_float() * 1024 });
		

		sprite.m_visible = true;
		sprite.m_internalVisible = true;
		sprite.m_depth = 0;
		sprite.m_rect = { 0.0f, 0.0f, 180.0f, 260.0f };
		sprite.m_color = { 255, 255, 255, 255 };
		sprite.m_origin = { 90.0f, 130.0f };

		// Add material for entity.
		mint::fx::CMaterialManager::Get().set_material_for_entity("mat_mounted_knight_default", entity);
		
		add_entity(entity);

		CWorldQuery::Get().register_entity_proxy(entity, CUCA::sprite_get_destination_rect(entity));
	}




 

	m_ready = true;
	return true;
}


void CMainScene::on_before_unload()
{

}


void CMainScene::on_unload()
{

}