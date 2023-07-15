#include "MainScene.h"

void CMainScene::on_update(mint::f32 dt /*= 0.0f*/)
{
	//CUCA::transform_rotate(m_knight,  mint::algorithm::degree_to_radians(45.0f * dt));

 	//CUCA::transform_translate(m_knight, { dt, 0.0f });

	if (mint::CSAS::Get().is_entity_visible(m_particle))
	{
		mint::fx::CParticleSystem::Get().set_particle_emitter_active(m_particle, true);
	}
	else
	{
		mint::fx::CParticleSystem::Get().set_particle_emitter_active(m_particle, false);
	}

// 	auto mouse = mint::CInput::get_mouse_position();
// 
// 	auto world_mouse = mint::fx::CCameraManager::Get().get_active_camera()->vector_screen_to_world(mouse);
// 
// 	CUCA::transform_set_position(m_particle, world_mouse);
// 
// 	auto emitter = mint::fx::CParticleSystem::Get().get_particle_emitter_for_entity(m_particle);
// 
// 	emitter->set_emitter_position(world_mouse);
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
	static mint::CRect rect = { 0.0f, 0.0f, 1.0f, 1.0f };
	b2AABB aabb = mint::algorithm::compute_aabb(rect);
	static Vec2 ray_start = { 0.0f, 0.0f };
	static Vec2 ray_end = { 1.0f, 1.0f };

	ImGuiID sid = 10000, scid = 20000;

	ImGui::Begin("World Query");

	CUI::edit_field_rect(rect, -2000.0f, 2000.0f, "Query Rectangle", "", sid++, scid++);

	u32 result = world::CWorldQuery::Get().get_entity_count_at_point_in_radius(rect);

	ImGui::Text(TextFormat("Count query AABB: %d", result));

	TestWorldQueryFilter filter;
	Vector< world::SWorldQueryProxy > result_entities = world::CWorldQuery::Get().get_entities_at_point_in_radius< TestWorldQueryFilter >(rect, &filter);

	for (auto& entity : result_entities)
	{
		ImGui::Text(TextFormat("Filter Query AABB: %zu (entt::entity)", (u64)entity.m_entity));
	}

	bool any_result = world::CWorldQuery::Get().get_any_entity_at_point_in_radius(rect);

	ImGui::Text(TextFormat("Any query AABB: %s", any_result == true ? "True" : "False"));


	ImGui::Separator();

	CUI::edit_field_vec2(ray_start, -2000.0f, 2000.0f, "Query ray start", "", sid++, scid++);
	CUI::edit_field_vec2(ray_end, -2000.0f, 2000.0f, "Query ray end", "", sid++, scid++);

	result = world::CWorldQuery::Get().get_entity_count_by_ray_intersection(ray_start, ray_end);

	ImGui::Text(TextFormat("Count query Raycast: %d", result));

	result_entities = world::CWorldQuery::Get().get_entities_by_ray_intersection< TestWorldQueryFilter >(ray_start, ray_end, &filter);

	for (auto& entity : result_entities)
	{
		ImGui::Text(TextFormat("Filter Query Raycast: %zu (entt::entity)", (u64)entity.m_entity));
	}

	any_result = world::CWorldQuery::Get().get_any_entity_ray_intersection(ray_start, ray_end);

	ImGui::Text(TextFormat("Any query Raycast: %s", any_result == true ? "True" : "False"));

	ImGui::End();


	fx::CCameraManager::Get().get_active_camera()->use_camera();

	// Render aabb, this is the same as rendering a rect computed from aabb.
	fx::CPrimitiveRenderer::render_aabb_filled(aabb, { 125, 125, 125, 30 });

	// Render rect.
	fx::CPrimitiveRenderer::render_rectangle_filled(rect, { 0, 255, 125, 30 });

	// Render raycast.
	fx::CPrimitiveRenderer::render_line(ray_start, ray_end, MINT_RED(), 2.0f);

 	fx::CCameraManager::Get().get_active_camera()->end_camera();
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
	using namespace mint::world;

	perform_all_database_tests();



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



	/*
	* TESTING SOUND ENGINE
	*/

	mint::sound::CSoundEngine::Get().set_listener_position({ 0, 0, -1 });
	mint::sound::CSoundEngine::Get().set_listener_forward({ 0, 0, 1 });
	mint::sound::CSoundEngine::Get().set_listener_velocity({ 0, 0, 0 });
	mint::sound::CSoundEngine::Get().set_listener_up({ 0, 1, 0 });

	m_sound = m_registry.create_entity();
	{
		auto& identifier = m_registry.add_component< mint::component::SIdentifier >(m_sound);
		auto& hierarchy = m_registry.add_component< mint::component::SSceneHierarchy >(m_sound);
		auto& transform = m_registry.add_component< mint::component::STransform >(m_sound);
		auto& sound = m_registry.add_component< mint::component::SSoundSource >(m_sound);
		auto& ws = m_registry.add_component< mint::component::SWorldSettings >(m_sound);

		ws.m_groupId = 2;
		ws.m_enabled = true;
		ws.m_filterEnabled = false;
		ws.m_queryable = false;

		identifier.m_enttId = SCAST(u64, m_sound);
		identifier.m_uuid = identifier.m_enttId;
		identifier.m_debugName = "SoundSource_MainTheme";
		hierarchy.m_parent = entt::null;


		mint::sound::CSoundEngine::Get().create_sound_source(m_sound, "main_theme_battle");

		// Transform data has to be set after creating the sound source in the sound engine.
		CUCA::transform_set_scale(m_sound, { 1.0f, 1.0f });
		CUCA::transform_set_rotation(m_sound, 0);
		CUCA::transform_set_position(m_sound, { 1, 1 });


		sound.m_minDistance = 0.5f;
		sound.m_maxDistance = 5000.0f;

		CUCA::soundsource_set_sound_source_mode(m_sound, FMOD_3D);
		CUCA::soundsource_set_sound_source_volume(m_sound, 1.0f);
		CUCA::soundsource_set_sound_source_pitch(m_sound, 0.0f);
		CUCA::soundsource_set_sound_source_pan(m_sound, 0.0f);
		CUCA::soundsource_set_sound_source_height(m_sound, 0.0f);
		CUCA::soundsource_set_sound_source_velocity(m_sound, { 1.0f, 1.0f, 0.0f });
		CUCA::soundsource_set_sound_source_cone_orientation(m_sound, { 0.0f, 0.0f, -1.0f });
		CUCA::soundsource_set_sound_source_cone_settings(m_sound, 90.0f, 180.0f, 1.0f);
		CUCA::soundsource_set_sound_source_loopmode(m_sound, 1);

		add_entity(m_sound);
	}

	m_forest = m_registry.create_entity();
	{
		auto& identifier = m_registry.add_component< mint::component::SIdentifier >(m_forest);
		auto& hierarchy = m_registry.add_component< mint::component::SSceneHierarchy >(m_forest);
		auto& transform = m_registry.add_component< mint::component::STransform >(m_forest);
		auto& sound = m_registry.add_component< mint::component::SSoundSource >(m_forest);
		auto& ws = m_registry.add_component< mint::component::SWorldSettings >(m_forest);

		ws.m_groupId = 2;
		ws.m_enabled = true;
		ws.m_filterEnabled = false;
		ws.m_queryable = false;

		identifier.m_enttId = SCAST(u64, m_forest);
		identifier.m_uuid = identifier.m_enttId;
		identifier.m_debugName = "SoundSource_Forest";
		hierarchy.m_parent = entt::null;


		mint::sound::CSoundEngine::Get().create_sound_source(m_forest, "main_theme_forest");

		// Transform data has to be set after creating the sound source in the sound engine.
		CUCA::transform_set_scale(m_forest, { 1.0f, 1.0f });
		CUCA::transform_set_rotation(m_forest, 0);
		CUCA::transform_set_position(m_forest, { 10, 15 });


		sound.m_minDistance = 10.0f;
		sound.m_maxDistance = 5000.0f;

		CUCA::soundsource_set_sound_source_mode(m_forest, FMOD_3D);
		CUCA::soundsource_set_sound_source_volume(m_forest, 1.0f);
		CUCA::soundsource_set_sound_source_pitch(m_forest, 0.0f);
		CUCA::soundsource_set_sound_source_pan(m_forest, 0.0f);
		CUCA::soundsource_set_sound_source_height(m_forest, 0.0f);
		CUCA::soundsource_set_sound_source_velocity(m_forest, { 1.0f, 1.0f, 0.0f });
		CUCA::soundsource_set_sound_source_cone_orientation(m_forest, { 0.0f, 0.0f, -1.0f });
		CUCA::soundsource_set_sound_source_cone_settings(m_forest, 90.0f, 180.0f, 1.0f);
		CUCA::soundsource_set_sound_source_loopmode(m_forest, 1);

		add_entity(m_forest);
	}


	m_shot = m_registry.create_entity();
	{
		auto& identifier = m_registry.add_component< mint::component::SIdentifier >(m_shot);
		auto& hierarchy = m_registry.add_component< mint::component::SSceneHierarchy >(m_shot);
		auto& transform = m_registry.add_component< mint::component::STransform >(m_shot);
		auto& sound = m_registry.add_component< mint::component::SSoundSource >(m_shot);
		auto& ws = m_registry.add_component< mint::component::SWorldSettings >(m_shot);

		ws.m_groupId = 2;
		ws.m_enabled = true;
		ws.m_filterEnabled = false;
		ws.m_queryable = false;

		identifier.m_enttId = SCAST(u64, m_shot);
		identifier.m_uuid = identifier.m_enttId;
		identifier.m_debugName = "SoundSource_GunShot";
		hierarchy.m_parent = entt::null;


		mint::sound::CSoundEngine::Get().create_sound_source(m_shot, "9mm_Spray");

		// Transform data has to be set after creating the sound source in the sound engine.
		CUCA::transform_set_scale(m_shot, { 1.0f, 1.0f });
		CUCA::transform_set_rotation(m_shot, 0);
		CUCA::transform_set_position(m_shot, { 10, 1 });


		sound.m_minDistance = 1.0f;
		sound.m_maxDistance = 5000.0f;

		CUCA::soundsource_set_sound_source_mode(m_shot, FMOD_3D);
		CUCA::soundsource_set_sound_source_volume(m_shot, 1.0f);
		CUCA::soundsource_set_sound_source_pitch(m_shot, 0.0f);
		CUCA::soundsource_set_sound_source_pan(m_shot, 0.0f);
		CUCA::soundsource_set_sound_source_height(m_shot, 0.0f);
		CUCA::soundsource_set_sound_source_velocity(m_shot, { 1.0f, 1.0f, 0.0f });
		CUCA::soundsource_set_sound_source_cone_orientation(m_shot, { 0.0f, 0.0f, -1.0f });
		CUCA::soundsource_set_sound_source_cone_settings(m_shot, 360.0f, 360.0f, 1.0f);
		CUCA::soundsource_set_sound_source_loopmode(m_shot, 1);

		add_entity(m_shot);
	}




	/*
	* TESTING PARTICLE SYSTEM
	*/
	m_particle = m_registry.create_entity();
 
 	auto& identifier = m_registry.add_component< mint::component::SIdentifier >(m_particle);
 	auto& hierarchy = m_registry.add_component< mint::component::SSceneHierarchy >(m_particle);
 	auto& transform = m_registry.add_component< mint::component::STransform >(m_particle);
	auto& particles = m_registry.add_component< mint::component::SParticleEmitter >(m_particle);
	auto& sprite = m_registry.add_component< mint::component::SSprite >(m_particle);
	auto& dynamic = m_registry.add_component< mint::component::SDynamicGameobject >(m_particle);
	auto& ws = m_registry.add_component< mint::component::SWorldSettings >(m_particle);
 
	ws.m_groupId = 1;
	ws.m_enabled = true;
	ws.m_filterEnabled = false;
	ws.m_queryable = false;

 	identifier.m_enttId = SCAST(u64, m_particle);
 	identifier.m_uuid = identifier.m_enttId;
 	identifier.m_debugName = "ParticleEmitter";
 	hierarchy.m_parent = entt::null;
 
	sprite.m_visible = true;
	sprite.m_internalVisible = true;
	sprite.m_depth = 0;
	sprite.m_rect = { 0.0f, 0.0f, 512.0f, 512.0f };
	sprite.m_color = { 255, 255, 255, 255 };
	sprite.m_origin = { 256.0f, 256.0f };

 	CUCA::transform_set_scale(m_particle, { 1.0f, 1.0f });
 	CUCA::transform_set_rotation(m_particle, 0);
 	CUCA::transform_set_position(m_particle, { 0, 0 });

	add_entity(m_particle);

	mint::fx::CParticleSystem::Get().request_entity_registration(m_particle);
	mint::fx::CParticleSystem::Get().set_entity_particle_emitter(m_particle, "part_testing_particle");
	


	mint::CRandom random;
	for (int i = 0; i < 1; i++)
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

		CWorldQuery::Get().register_entity_proxy(entity, CUCA::sprite_get_destination_rect(entity), CUCA::identifier_get_debug_name(entity));
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

void CMainScene::setup_test_database(mint::world::CDatabase& db)
{
	using namespace mint::world;
	using namespace mint;

	u64 node_id = 1000;
	u64 edge_id = 2000;
	db.create_node(node_id++, "Player", mint::world::NodeType_Entity, m_knight);
	db.create_node(node_id++, "Walter", mint::world::NodeType_Entity);
	db.create_node(node_id++, "Hans", mint::world::NodeType_Entity);

	db.create_node(node_id++, "Friendly", mint::world::NodeType_Attitude);
	db.create_node(node_id++, "Raider", mint::world::NodeType_Membership);
	db.create_node(node_id++, "NPC", mint::world::NodeType_Classification);

	db.create_edge("Player", "Friendly", edge_id++, "Attitude", 27.0f);
	db.create_edge("Player", "Walter", edge_id++, "Like", 13.3f);
	db.create_edge("Player", "Hans", edge_id++, "Like", 77.5f);

	db.create_edge("Walter", "Friendly", edge_id++, "Attitude", 27.0f);
	db.create_edge("Walter", "Raider", edge_id++, "Raider", 100.0f);
	db.create_edge("Walter", "NPC", edge_id++, "NPC", 100.0f);
	db.create_edge("Walter", "Player", edge_id++, "Like", 15.2f);

	db.create_edge("Hans", "NPC", edge_id++, "NPC", 100.0f);
	db.create_edge("Hans", "Raider", edge_id++, "Raider", 100.0f);
}

void CMainScene::perform_all_database_tests()
{
	using namespace mint::world;
	using namespace mint;

	CDatabase db; db.initialize(MINT_ENTITY_COUNT_MAX);

	setup_test_database(db);


	auto result = db.query_get_all_object_subject_edges("Player", "Like", 10.0f, mint::world::LogicalWeightOperator_Greater);

	MINT_LOG_INFO("[Database Test] Get all that \"Player\" \"Like\"s for at least 10.0f:");
	for (auto& edge : result)
	{
		MINT_LOG_INFO("\tLabel=\"{}\", Weight=\"{}\", From=\"{}\", To=\"{}\"", edge.get_label(), edge.get_weight(), edge.get_from_node()->get_label(), edge.get_to_node()->get_label());
	}
	MINT_LOG_SEPARATOR();



	result = db.query_get_all_subject_object_edges("Like", "Walter");

	MINT_LOG_INFO("[Database Test] Get all those who \"Like\" \"Walter\" for any amount:");
	for (auto& edge : result)
	{
		MINT_LOG_INFO("\tLabel=\"{}\", Weight=\"{}\", From=\"{}\", To=\"{}\"", edge.get_label(), edge.get_weight(), edge.get_from_node()->get_label(), edge.get_to_node()->get_label());
	}
	MINT_LOG_SEPARATOR();



	result = db.query_get_all_subject_object_edges("*", "Walter");

	MINT_LOG_INFO("[Database Test] Get all those who have any relationship to \"Walter\" for any amount:");
	for (auto& edge : result)
	{
		MINT_LOG_INFO("\tLabel=\"{}\", Weight=\"{}\", From=\"{}\", To=\"{}\"", edge.get_label(), edge.get_weight(), edge.get_from_node()->get_label(), edge.get_to_node()->get_label());
	}
	MINT_LOG_SEPARATOR();


	TestWorldQueryDatabaseFilter filter("Attitude", m_knight);
	result = db.query_get_all_object_subject_edges("Player", "*", 0.0f, mint::world::LogicalWeightOperator_None, &filter);

	MINT_LOG_INFO("[Database Test] Get all those to which \"Player\" has any relationship for any amount:");
	for (auto& edge : result)
	{
		MINT_LOG_INFO("\tLabel=\"{}\", Weight=\"{}\", From=\"{}\", To=\"{}\"", edge.get_label(), edge.get_weight(), edge.get_from_node()->get_label(), edge.get_to_node()->get_label());
	}
	MINT_LOG_SEPARATOR();


	db.remove_edge(1000, 2000);
	db.remove_edge(2001);
	db.remove_node(1000);


	result = db.query_get_all_subject_object_edges("Like", "Hans");

	MINT_LOG_INFO("[Database Test] Get all those who \"Like\" \"Hans\" for any amount:");
	for (auto& edge : result)
	{
		MINT_LOG_INFO("\tLabel=\"{}\", Weight=\"{}\", From=\"{}\", To=\"{}\"", edge.get_label(), edge.get_weight(), edge.get_from_node()->get_label(), edge.get_to_node()->get_label());
	}
	MINT_LOG_SEPARATOR();
}

void CMainScene::perform_database_test(mint::world::CDatabase& db, mint::Vector< mint::world::SToken >& bytecode, const mint::String& description)
{
}
