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
 	m_knight = m_registry.create_entity();
 
 	auto& identifier = m_registry.add_component< mint::component::SIdentifier >(m_knight);
 	auto& hierarchy = m_registry.add_component< mint::component::SSceneHierarchy >(m_knight);
 	auto& transform = m_registry.add_component< mint::component::STransform >(m_knight);
 	auto& sprite = m_registry.add_component< mint::component::SSprite >(m_knight);
 	auto& script = m_registry.add_component< mint::component::SScript >(m_knight);
 	auto& dynamic = m_registry.add_component< mint::component::SDynamicGameobject >(m_knight);
 
 
 	identifier.m_enttId = SCAST(u64, m_knight);
 	identifier.m_uuid = identifier.m_enttId;
 	identifier.m_debugName = "Gopnik";
 	hierarchy.m_parent = entt::null;
 
 	CUCA::transform_set_scale(m_knight, { 1.0f, 1.0f });
 	CUCA::transform_set_rotation(m_knight, 0);
 	CUCA::transform_set_position(m_knight, { 0, 0 });
 
 
 	sprite.m_visible = true;
 	sprite.m_internalVisible = true;
 	sprite.m_depth = 0;
 	sprite.m_rect = { 0.0f, 0.0f, 0.0f, 0.0f };
 	sprite.m_color = { 255, 255, 255, 255 };
 	sprite.m_origin = { 0.0f, 0.0f };

 
 
 	// Set script for entity.
 	mint::scripting::CBehaviorEngine::Get().set_behavior_for_entity("SoldierController", m_knight);
 

    // Set animator for entity.
	animation::CAnimationSystem::Get().request_entity_registration(m_knight);

	animation::CAnimationSystem::Get().set_entity_animation(m_knight, "anim_gopnik_idle");


// 
// 	// Frame animator.
// 	animation::CAnimationSystem::Get().add_entity_animation_state(m_knight, "Default");
// 
// 	animation::CAnimationSystem::Get().set_entity_animation_state(m_knight, "Default");
// 
// 	// Set animation frame data.
// 	auto data = new mint::animation::frameanim::SFrameAnimationBehaviorData();
// 	data->set_frame_count_x(6);
// 	data->set_frame_count_y(1);
// 	for (auto y = 0; y < 1; y++)
// 	{
// 		for (auto x = 0; x < 6; x++)
// 		{
// 			data->add_keyframe(x, y);
// 		}
// 	}
// 
// 	auto animator = animation::CAnimationSystem::Get().try_push_entity_animator< mint::animation::frameanim::SFrameAnimationBehaviorData >(m_knight, "Default", "FrameAnimator", data, 
// 		mint::animation::frameanim::on_animation_update,
// 		mint::animation::frameanim::on_animation_enter,
// 		mint::animation::frameanim::on_animation_exit,
// 		mint::animation::frameanim::on_animator_initialize,
// 		mint::animation::frameanim::on_animator_terminate);
// 
// 	// Set animator required data.
// 	animator->set_animation_duration(1);
// 
// 	animator->set_animation_easing_function(bx::Easing::SmoothStep);
// 
// 	animator->set_animation_speed(5.0f);
// 
// 	animator->set_animation_material("mat_gopnik_idle");
// 
// 
// 	// Color animator.
// 	// Set animator data.
// 	auto cdata = new mint::animation::coloranim::SColorAnimationBehaviorData();
// 	cdata->set_base_color(MINT_RED_LIGHT());
// 	cdata->set_destination_color(MINT_GREEN());
// 
// 	auto color_animator = animation::CAnimationSystem::Get().try_push_entity_animator< mint::animation::coloranim::SColorAnimationBehaviorData >(m_knight, "Default", "ColorAnimator", cdata,
// 		mint::animation::coloranim::on_animation_update,
// 		mint::animation::coloranim::on_animation_enter,
// 		mint::animation::coloranim::on_animation_exit,
// 		mint::animation::coloranim::on_animator_initialize,
// 		mint::animation::coloranim::on_animator_terminate);
// 
// 	color_animator->set_animation_duration(25);
// 
// 	color_animator->set_animation_easing_function(bx::Easing::SmoothStep);
// 
// 	color_animator->set_animation_speed(1.0f);
// 
// 
// 	// Translation animator.
// 	auto tdata = new mint::animation::translationanim::STranslationAnimationBehaviorData();
// 	tdata->set_base_translation({ 0.0f, -100.0f });
// 	tdata->set_destination_translation({ 0.0f, 100.0f });
// 
// 	auto translation_animator = animation::CAnimationSystem::Get().try_push_entity_animator< mint::animation::translationanim::STranslationAnimationBehaviorData >(m_knight, "Default", "TranslationAnimator", tdata,
// 		mint::animation::translationanim::on_animation_update,
// 		mint::animation::translationanim::on_animation_enter,
// 		mint::animation::translationanim::on_animation_exit,
// 		mint::animation::translationanim::on_animator_initialize,
// 		mint::animation::translationanim::on_animator_terminate);
// 
// 	translation_animator->set_animation_duration(5);
// 
// 	translation_animator->set_animation_easing_function(bx::Easing::SmoothStep);
// 
// 	translation_animator->set_animation_speed(1.0f);
// 
// 
// 	// Scale animator.
// 	auto sdata = new mint::animation::scaleanim::SScaleAnimationBehaviorData();
// 	sdata->set_base_scale({ 0.5f, 0.5f });
// 	sdata->set_destination_scale({ 5.0f, 5.0f });
// 
// 	auto scale_animator = animation::CAnimationSystem::Get().try_push_entity_animator< mint::animation::scaleanim::SScaleAnimationBehaviorData >(m_knight, "Default", "ScaleAnimator", sdata,
// 		mint::animation::scaleanim::on_animation_update,
// 		mint::animation::scaleanim::on_animation_enter,
// 		mint::animation::scaleanim::on_animation_exit,
// 		mint::animation::scaleanim::on_animator_initialize,
// 		mint::animation::scaleanim::on_animator_terminate);
// 
// 	scale_animator->set_animation_duration(5);
// 
// 	scale_animator->set_animation_easing_function(bx::Easing::SmoothStep);
// 
//  	scale_animator->set_animation_speed(1.0f);


  	add_entity(m_knight);

 
	/*
		TESTING REFLECTION
	*/
#if MINT_DISTR
#else
	auto& reflected = m_registry.add_component< MyComponent >(m_knight);
	reflected.m_entity = SCAST(u64, m_knight);
	reflected.m_name = identifier.m_debugName;
	reflected.m_isReflected = true;
#endif





	mint::CRandom random;
	for (int i = 0; i < 2; i++)
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
		CUCA::transform_set_rotation(entity, random.normalized_float() * 360);
		CUCA::transform_set_position(entity, { random.normalized_float() * 512, random.normalized_float() * 512 });

		sprite.m_visible = true;
		sprite.m_internalVisible = true;
		sprite.m_depth = 0;
		sprite.m_rect = { 0.0f, 0.0f, 180.0f, 180.0f };
		sprite.m_color = { 255, 255, 255, 255 };
		sprite.m_origin = { 90.0f, 90.0f };



		mint::fx::SMaterialDefinition def;

		// Set material data and bind static uniforms once.
		def.m_materialName = "Default Material";
		def.m_shaderProgramName = "Sprite";
		def.m_textureName = "Samurai";
		def.m_blendMode = BlendMode::BLEND_ALPHA;
		def.m_srcBlendFactor = mint::fx::BlendingFactor_SrcAlpha;
		def.m_dstBlendFactor = mint::fx::BlendingFactor_OneMinusDstAlpha;
		def.m_blendingEquation = mint::fx::BlendingEquation_Max;

		// Add material for entity.
		mint::fx::CMaterialManager::Get().add_material_for_entity(entity, def);


		mint::fx::SMaterialDefinition sdef;

		// Set material data and bind static uniforms once.
		sdef.m_materialName = "Second Material";
		sdef.m_shaderProgramName = "Sprite";
		sdef.m_textureName = "Samurai";
		sdef.m_blendMode = BlendMode::BLEND_ADD_COLORS;
		sdef.m_srcBlendFactor = mint::fx::BlendingFactor_SrcAlpha;
		sdef.m_dstBlendFactor = mint::fx::BlendingFactor_DstColor;
		sdef.m_blendingEquation = mint::fx::BlendingEquation_BlendColor;


		// Add material for entity.
		mint::fx::CMaterialManager::Get().add_material_for_entity(entity, sdef);


		add_entity(entity);
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