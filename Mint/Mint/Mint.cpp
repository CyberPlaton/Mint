#include "Mint.h"

namespace mint
{
	
	bool CMintEngine::initialize(const String& manifest_filepath)
	{
		_prepare_for_init();

		CScene* initial_scene = nullptr;
		CWindow::SDescription wdesc;
		CPhysicsSystem::SDescription pdesc;
		maml::CDocument document;


		if(!CSerializer::load_maml_document(manifest_filepath, document)) return false;

		if(!_load_app_manifiest(document, wdesc, pdesc, initial_scene)) return false;

		if(!_pre_init(wdesc, pdesc)) return false;

		// GPU context can only be reported after the window and raylib have been initialized.
		CGlobalGraphicsSettings::Get().print_graphics_context();


		if(!_init()) return false;

		if(!_post_init(initial_scene)) return false;

		// Finalize Plugin initialization.
		CPluginSystem::Get().finalize_initialize();

		// Show plugins that were initialized and are now active and those that failed initialization.
		CPluginSystem::Get().print_active_and_failed_plugins();


		CSceneManager::Get().set_initial_scene(initial_scene);


		// Create the anti-aliasing Post-Processing renderer.
		fx::CRenderingPassStack::Get().try_push_rendering_pass(new fx::CFXAA());

		fx::CRenderingPassStack::Get().try_push_rendering_pass(new CWorldQueryDebugRender());

		fx::CRenderingPassStack::Get().try_push_rendering_pass(new CSoundSystemDebugRender());


		// Rendering Stack can be reported after context was initialized and the renderers created.
		// Please note that it is possible for renderers to be dependent on the Scene being initialized.
		print_engine_rendering_pass_stack();
		print_engine_registered_importers_and_exporters();


		m_running = true;

		return true;
	}


	void CMintEngine::terminate()
	{
		_pre_terminate();

		_terminate();

		_post_terminate();

		_cleanup_after_terminate();
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


	void CMintEngine::print_engine_context()
	{
		MINT_LOG_INFO("Runtime version: \"{}\"", MINT_ENGINE_VERSION_STRING);
		MINT_LOG_SEPARATOR();
	}


	void CMintEngine::begin_rendering()
	{
		fx::CRenderingPassStack::Get().on_begin_drawing();
	}


	void CMintEngine::on_update(f32 dt)
	{
		MINT_ACTIVE_SCENE()->on_update(dt);

		sound::CSoundEngine::Get().on_update(dt);

		CPhysicsSystem::Get().update(dt);

		CSceneManager::Get().update();

		CPluginSystem::Get().on_update(dt);

		fx::CCameraManager::Get().on_update(dt);

		scripting::CScriptEngine::Get().set_should_update(true);

		scripting::CBehaviorEngine::Get().set_should_update(true);

		animation::CAnimationSystem::Get().set_should_update(true);

		fx::CParticleSystem::Get().on_update(dt);
	}


	void CMintEngine::frame()
	{
		auto scene = MINT_ACTIVE_SCENE();
		auto camera = fx::CCameraManager::Get().get_active_camera();
		auto& frame_entities = CSAS::Get().retrieve_visible_entities();

		fx::CRenderingPassStack::Get().on_frame(camera, frame_entities);
	}


	void CMintEngine::ui_frame_begin()
	{
		fx::CRenderingPassStack::Get().on_ui_frame();

		CUI::Get().begin();
	}

	void CMintEngine::ui_frame_render()
	{
		MINT_ACTIVE_SCENE()->on_ui_render(get_engine_frametime());
	}

	void CMintEngine::ui_frame_end()
	{
		CUI::Get().end();

		fx::CRenderingPassStack::Get().on_ui_frame_end();
	}


	void CMintEngine::end_frame()
	{
		fx::CRenderingPassStack::Get().on_frame_end();
	}


	void CMintEngine::end_rendering()
	{
		fx::CRenderingPassStack::Get().on_end_drawing();
	}


	void CMintEngine::on_before_update()
	{
		CEventSystem::Get().update();


		// Populate spatial acceleration structure with current dynamic entities and let it update the Quad Tree.
		MINT_ACTIVE_SCENE()->submit_dynamic_entities();

		CSAS::Get().set_should_update(true);


		CPluginSystem::Get().on_before_update();
	}


	void CMintEngine::on_after_update(f32 dt)
	{
		CPluginSystem::Get().on_after_update(dt);
	}


	const mint::CWindow& CMintEngine::get_main_window_const() const
	{
		return m_mainWindow;
	}


	mint::CWindow& CMintEngine::get_main_window_ref()
	{
		return m_mainWindow;
	}


	mint::f32 CMintEngine::get_engine_fps()
	{
		return m_mainTimestep.get_fps();
	}


	mint::f32 CMintEngine::get_engine_frametime()
	{
		return m_mainTimestep.get_frametime();
	}


	void CMintEngine::set_engine_fps(f32 fps)
	{
		m_mainTimestep.set_fps(fps);
	}


	void CMintEngine::set_engine_window_title(const String& title)
	{
		m_mainWindow.set_title(title);
	}


	void CMintEngine::print_engine_rendering_pass_stack()
	{
		fx::CRenderingPassStack::Get().print_rendering_pass_stack();
	}


	void CMintEngine::print_engine_registered_importers_and_exporters()
	{
		Vector< u64 > hash_values;

		MINT_LOG_INFO("Dumping registered component importer and exporter:");
		MINT_LOG_INFO("\timporters:");
		IScene::get_all_component_importer_hash_value(hash_values);
		for (auto i : hash_values)
		{
			MINT_LOG_INFO("\t\thash: {}", i);
		}

		hash_values.clear();

		MINT_LOG_INFO("\texporters:");
		IScene::get_all_component_exporter_hash_value(hash_values);
		for (auto i : hash_values)
		{
			MINT_LOG_INFO("\t\thash: {}", i);
		}

		MINT_LOG_SEPARATOR();
	}

	bool CMintEngine::is_context_windows()
	{
#if MINT_PLATFORM_WINDOWS
		return true;
#endif

		return false;
	}

	bool CMintEngine::is_context_linux()
	{
#if MINT_PLATFORM_LINUX
		return true;
#endif

		return false;
	}

	bool CMintEngine::is_build_debug()
	{
#if MINT_DEBUG
		return true;
#endif

		return false;
	}

	
	bool CMintEngine::is_build_release()
	{
#if MINT_RELEASE
		return true;
#endif

		return false;
	}

	bool CMintEngine::is_build_distr()
	{
#if MINT_DISTR
		return true;
#endif

		return false;
	}

	bool CMintEngine::_prepare_for_init()
	{
		IMintEngine::initialize_cuca_critical_sections();
		
		bool result = true;


		// Initialize lowest level sub-systems.
		
		// IScene.
		result &= IScene::initialize();
		
		// Logging.
		result &= CLogging::Get().initialize();

		// Function and Memory Profiler.
#if MINT_PROFILE
		result &= profiler::CFunctionProfiler::Get().initialize();

		if (result)
		{
			profiler::CFunctionProfiler::Get().set_update_interval_in_seconds(3);
			profiler::CFunctionProfiler::Get().run_profiler_thread();
		}
#else
		MINT_LOG_WARN("[{:.4f}][CMintEngine::_prepare_for_init] Function and Memory profiling disabled!", MINT_APP_TIME);
#endif

		// Fatal error! We cant even report what happened.
		if (!result) return false;


		// Initialize component exporters and importers.
		IMintEngine::register_component_exporter_functions();
		IMintEngine::register_component_importer_functions();


		// Initialize ressource loaders.
		CRessourceLoaderFactory::register_ressource_loader("Texture", &CTextureLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Shader", &CShaderLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Script", &CScriptLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Behavior", &CBehaviorLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Material", &CMaterialLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Animation", &CAnimationLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("ParticleEmitter", &CParticleEmitterLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Sound", &CSoundLoader::create);



		// Global OS settings.
		result &= CGlobalOSSettings::Get().initialize();

		// Global Graphics settings. Can be done here as we do not use any OpenGL functions there.
		result &= CGlobalGraphicsSettings::Get().initialize();

		// Global CPU settings.
		result &= CGlobalCPUSettings::Get().initialize();

		// Report engine context.
		MINT_ENGINE()->print_engine_context();
		CGlobalOSSettings::Get().print_os_context();
		CGlobalCPUSettings::Get().print_cpu_context();

		// Initialize Plugin system and show registered plugins.
		result &= CPluginSystem::Get().initialize();

		CPluginSystem::Get().print_registered_plugins();


		return result;
	}


	bool CMintEngine::_load_app_manifiest(maml::CDocument& document, CWindow::SDescription& window_desc, CPhysicsSystem::SDescription& physics_desc, CScene*& initial_scene)
	{
		auto root = document.get_root();

		bool window_data_loaded = false;
		bool scene_data_loaded = false;

		auto window_node = document.find_first_match_in_document("window");
		auto physics_node = document.find_first_match_in_document("physics");
		auto package_manifest_node = document.find_first_match_in_document("packageManifest");
		auto initial_scene_node = document.find_first_match_in_document("initialScene");

		if(window_node)
		{
			CSerializer::import_string(window_desc.m_title, "title", window_node);
			CSerializer::import_string(window_desc.m_window_icon_path, "icon", window_node);
			CSerializer::import_bool(&window_desc.m_fullscreen, "fullscreen", window_node);
			CSerializer::import_bool(&window_desc.m_resizable, "resizable", window_node);
			CSerializer::import_bool(&window_desc.m_decorated, "decorated", window_node);
			CSerializer::import_bool(&window_desc.m_show, "show", window_node);
			CSerializer::import_bool(&window_desc.m_minimized, "minimized", window_node);
			CSerializer::import_bool(&window_desc.m_runMinimized, "runMinimized", window_node);
			CSerializer::import_bool(&window_desc.m_focus, "focus", window_node);
			CSerializer::import_bool(&window_desc.m_vsync, "VSYNC", window_node);

			CSerializer::import_uint(&window_desc.m_width, "width", window_node);
			CSerializer::import_uint(&window_desc.m_height, "height", window_node);
			CSerializer::import_uint(&window_desc.m_targetFPS, "targetFPS", window_node);

			window_data_loaded = true;
		}
		if (physics_node)
		{
			CSerializer::import_float(&physics_desc.m_fixedTimestep, "fixedTimestep", physics_node);
			CSerializer::import_uint(&physics_desc.m_positionIterations, "positionIterations", physics_node);
			CSerializer::import_uint(&physics_desc.m_velocityIterations, "velocityIterations", physics_node);
			CSerializer::import_vec2(physics_desc.m_gravity, "gravity", physics_node);
		}
		if (package_manifest_node)
		{
		}
		if (initial_scene_node)
		{
			String scene_name;

			CSerializer::import_string(scene_name, "scene", initial_scene_node);

			initial_scene = CSceneFactory::create_scene(scene_name);

			if(!initial_scene)
			{
				return false;
			}

			IScene::set_active_scene(initial_scene);

			scene_data_loaded = true;
		}


		return scene_data_loaded && window_data_loaded;
	}


	bool CMintEngine::_init()
	{
		// Initialize engine sub systems.
		bool result = true;

		// UI.
		result &= CUI::Get().initialize();

		if (result)
		{
			const auto& window = get_main_window_const();
			CUI::Get().set_window_dimension({ window.get_w(), window.get_h() });
		}

		// Embedded shaders.
		result &= fx::CEmbeddedShaders::Get().initialize();

		// Renderers and Renderer Stack.
		result &= fx::CRenderingPassStack::Get().initialize();
		if (result)
		{
			fx::CRenderingPassStack::Get().try_push_rendering_pass(new fx::CSceneRenderer());

#if MINT_DISTR
#else
			fx::CRenderingPassStack::Get().try_push_rendering_pass(new fx::CDebugRenderer());
#endif
		}

		// Camera System.
		result &= mint::fx::CCameraManager::Get().initialize();

		// Particle System.
		result &= fx::CRenderingPassStack::Get().try_push_rendering_pass(&fx::CParticleSystem::Get());

		// Registry.
		result &= MINT_SCENE_REGISTRY()->initialize();

		// Scripting and Behavior engine.
		result &= scripting::CBehaviorEngine::Get().initialize();
		result &= scripting::CScriptEngine::Get().initialize();

		// Sound engine.
		result &= sound::CSoundEngine::Get().initialize();

		if(result)
		{
			// Plugin System (initialize plugins).
			CPluginSystem::Get().on_initialization();

			scripting::CBehaviorEngine::Get().run_behavior_engine_thread();
			scripting::CScriptEngine::Get().run_script_engine_thread();
		}

		return result;
	}


	bool CMintEngine::_pre_init(CWindow::SDescription& wdesc, CPhysicsSystem::SDescription& pdesc)
	{
		if (!m_mainWindow.initialize(wdesc)) return false;
		if (!CPhysicsSystem::Get().initialize(pdesc)) return false;

		set_engine_fps(SCAST(f32, wdesc.m_targetFPS));

		// Initialize medium level engine systems.
		bool result = true;

		// Event System.
		result &= CEventSystem::Get().initialize();

		// Shader manager.
		result &= CShaderManager::Get().initialize();

		// Texture manager.
		result &= CTextureManager::Get().initialize();

		// Material manager.
		result &= fx::CMaterialManager::Get().initialize();

		// Plugin Manager (pre-initialize plugins).
		CPluginSystem::Get().on_pre_initialization();

		// Scene Manager.
		result &= CSceneManager::Get().initialize();

		// World Query Interface.
		result &= world::CWorldQuery::Get().initialize();

		// Service Locator.
		result &= CServiceLocator::Get().initialize();

		if(result)
		{
			IService::register_service(new mint::scripting::CScriptLuaBindingService(), "LuaRegistration");
		}

		return result;
	}


	bool CMintEngine::_post_init(IScene* current_scene)
	{
		// Initialize high level engine sub systems.
		bool result = true;

		// Parallax.

		// Lighting.

		// Animation System.
		result &= animation::CAnimationSystem::Get().initialize();
		if (result)
		{
			animation::CAnimationSystem::Get().run_animation_system_thread();
		}

		// CSAS.
		result &= CSAS::Get().initialize();
		if(result)
		{
			CSAS::Get().run_sas_thread();
		}

		CPluginSystem::Get().on_post_initialization();

		return result;
	}


	void CMintEngine::_cleanup_after_terminate()
	{
		IMintEngine::delete_cuca_critical_sections();
	}


	void CMintEngine::_pre_terminate()
	{
		// Prepare plugin shutdown and terminate high level engine sub systems.
		CPluginSystem::Get().on_pre_termination();

		// Spatial acceleration structure.
		CSAS::Get().stop_sas_thread();
		CSAS::Get().terminate();

		// Particle System.
		fx::CParticleSystem::Get().terminate();

		// Renderers and Renderer Stack.
		fx::CRenderingPassStack::Get().terminate();

		// Animation system.
		animation::CAnimationSystem::Get().stop_animation_system_thread();
		animation::CAnimationSystem::Get().terminate();

		// Physics system.
		CPhysicsSystem::Get().terminate();
	}


	void CMintEngine::_terminate()
	{
		// Terminate plugins and medium level engine sub systems.
		CPluginSystem::Get().on_termination();

		// Camera System.
		mint::fx::CCameraManager::Get().terminate();

		// Sound engine.
		sound::CSoundEngine::Get().terminate();

		// Scripting engine.
		scripting::CBehaviorEngine::Get().stop_behavior_engine_thread();
		scripting::CScriptEngine::Get().stop_script_engine_thread();
		
		scripting::CBehaviorEngine::Get().terminate();
		scripting::CScriptEngine::Get().terminate();

		// Material manager.
		fx::CMaterialManager::Get().terminate();

		// Texture Manager.
		CTextureManager::Get().terminate();

		// Shader Manager.
		CShaderManager::Get().terminate();

		// UI.
		CUI::Get().terminate();

		// Embedded shaders.
		fx::CEmbeddedShaders::Get().terminate();
	}


	void CMintEngine::_post_terminate()
	{
		// Terminate plugins and lowest level engine sub systems.
		CPluginSystem::Get().on_post_termination();

		// World Query Interface.
		world::CWorldQuery::Get().terminate();

		// Scene Manager.
		CSceneManager::Get().terminate();

		// Plugin Manager.
		CPluginSystem::Get().terminate();

		// Event Manager.
		CEventSystem::Get().terminate();

		// Function and Memory profiler.
#if MINT_PROFILE
		profiler::CFunctionProfiler::Get().stop_profiler_thread();
		profiler::CFunctionProfiler::Get().terminate();
#endif

		// Logging.
		CLogging::Get().terminate();

		// IScene.
		IScene::terminate();
	}



}