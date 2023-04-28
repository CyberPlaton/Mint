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

		if(!_init()) return false;

		if(!_post_init(initial_scene)) return false;


		CSceneManager::Get().set_initial_scene(initial_scene);


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


	void CMintEngine::begin_rendering()
	{
		BeginDrawing();
	}


	void CMintEngine::on_update(f32 dt)
	{
		if (CPhysicsSystem::get_use_physics()) CPhysicsSystem::Get().update(dt);

		MINT_ACTIVE_SCENE()->on_update(dt);

		CSceneManager::Get().update();

		CPluginSystem::Get().on_update(dt);

		scripting::CScriptEngine::Get().set_should_update(true);

		scripting::CBehaviorEngine::Get().set_should_update(true);
	}


	void CMintEngine::begin_frame()
	{
		auto scene = MINT_ACTIVE_SCENE();
		auto camera = scene->get_active_camera();

		fx::CSceneRenderer::Get().on_pre_render(camera);
	}


	void CMintEngine::frame()
	{
		fx::CSceneRenderer::Get().on_render(MINT_ACTIVE_SCENE());
	}


	void CMintEngine::ui_frame_begin()
	{
		CUI::Get().begin();
	}

	void CMintEngine::ui_frame_render()
	{
		MINT_ACTIVE_SCENE()->on_ui_render(get_engine_frametime());
	}

	void CMintEngine::ui_frame_end()
	{
		CUI::Get().end();
	}


	void CMintEngine::end_frame()
	{
		fx::CSceneRenderer::Get().on_post_render();
	}


	void CMintEngine::end_rendering()
	{
		EndDrawing();
	}


	void CMintEngine::on_before_update()
	{
		CEventSystem::Get().update();

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


	bool CMintEngine::_prepare_for_init()
	{
		IMintEngine::initialize_cuca_critical_sections();
		
		bool result = true;


		// Initialize lowest level sub-systems.
		result &= CLogging::Get().initialize();

		// Initialize component exporters and importers.
		IMintEngine::register_component_exporter_functions();
		IMintEngine::register_component_importer_functions();


		// Initialize ressource loaders.
		CRessourceLoaderFactory::register_ressource_loader("Texture", &CTextureLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Shader", &CShaderLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Script", &CScriptLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Behavior", &CBehaviorLoader::create);

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
			bool use = false;

			CSerializer::import_bool(&use, "use", physics_node);
			if(use)
			{
				CSerializer::import_bool(&physics_desc.m_debugRender, "debugRender", physics_node);
				CSerializer::import_float(&physics_desc.m_fixedTimestep, "fixedTimestep", physics_node);
				CSerializer::import_uint(&physics_desc.m_positionIterations, "positionIterations", physics_node);
				CSerializer::import_uint(&physics_desc.m_velocityIterations, "velocityIterations", physics_node);
				CSerializer::import_vec2(physics_desc.m_gravity, "gravity", physics_node);
			
				CPhysicsSystem::set_use_physics(true);
			}
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

		// Embedded shaders.
		result &= fx::CEmbeddedShaders::Get().initialize();

		// Registry.
		result &= MINT_SCENE_REGISTRY().initialize();

		// Scripting and Behavior engine.
		result &= scripting::CBehaviorEngine::Get().initialize();
		result &= scripting::CScriptEngine::Get().initialize();

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
		if (CPhysicsSystem::get_use_physics() && !CPhysicsSystem::Get().initialize(pdesc)) return false;

		set_engine_fps(SCAST(f32, wdesc.m_targetFPS));

		// Initialize medium level engine systems.
		bool result = true;

		// Event System.
		result &= CEventSystem::Get().initialize();
		
		// Shader manager.
		result &= CShaderManager::Get().initialize();

		// Texture manager.
		result &= CTextureManager::Get().initialize();

		// Plugin Manager (pre-initialize plugins).
		CPluginSystem::Get().on_pre_initialization();

		// Scene Manager.
		result &= CSceneManager::Get().initialize();

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

		// Renderer.
		result &= fx::CSceneRenderer::Get().initialize();

		// CSAS.
		result &= CSAS::Get().initialize();
		if(result)
		{
			CSAS::Get().run_sas_thread();
		}


		CPluginSystem::Get().finalize_initialize();


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


		// Scene rendering.
		fx::CSceneRenderer::Get().terminate();

		// Animation system.
	}


	void CMintEngine::_terminate()
	{
		// Terminate plugins and medium level engine sub systems.
		CPluginSystem::Get().on_termination();


		// Scripting engine.
		scripting::CBehaviorEngine::Get().stop_behavior_engine_thread();
		scripting::CScriptEngine::Get().stop_script_engine_thread();
		
		scripting::CBehaviorEngine::Get().terminate();
		scripting::CScriptEngine::Get().terminate();


		// Texture Manager.
		CTextureManager::Get().terminate();

		// Shader Manager.
		CShaderManager::Get().terminate();

		// UI.

		// Embedded shaders.
		fx::CEmbeddedShaders::Get().terminate();
	}


	void CMintEngine::_post_terminate()
	{
		// Terminate plugins and lowest level engine sub systems.
		CPluginSystem::Get().on_post_termination();

		// Scene Manager.
		CSceneManager::Get().terminate();

		// Plugin Manager.
		CPluginSystem::Get().terminate();

		// Event Manager.
		CEventSystem::Get().terminate();

		// Logging.
		CLogging::Get().terminate();
	}



}