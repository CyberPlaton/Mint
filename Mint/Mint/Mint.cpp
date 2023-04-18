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

		if(!CSerializer::load_maml_document(manifest_filepath, document))
		{
			return false;
		}


		if(!_load_app_manifiest(document, wdesc, pdesc, initial_scene))
		{
			return false;
		}


		if(!_pre_init(wdesc, pdesc))
		{
			return false;
		}


		if(!_init())
		{
			return false;
		}


		if(!_post_init(initial_scene))
		{
			return false;
		}

		CPluginSystem::Get().finalize_initialize();

		CSceneManager::Get().set_initial_scene(initial_scene);


		m_running = true;

		return true;
	}


	void CMintEngine::terminate()
	{
		
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


	void CMintEngine::on_update(f32 dt)
	{
		_on_update(dt);
	}


	void CMintEngine::begin_frame()
	{
		_begin_frame();
	}


	void CMintEngine::frame()
	{
		_frame();
	}


	void CMintEngine::ui_frame_begin()
	{
		CUI::Get().begin();
	}


	void CMintEngine::ui_frame_end()
	{
		CUI::Get().end();
	}


	void CMintEngine::end_frame()
	{
		_end_frame();
	}


	void CMintEngine::on_before_update()
	{
		_on_pre_update();
	}


	void CMintEngine::on_after_update(f32 dt)
	{
		_on_post_update();
	}


	mint::CWindow& CMintEngine::get_main_window()
	{
		return m_mainWindow;
	}



	bool CMintEngine::_prepare_for_init()
	{
		IMintEngine::initialize_cuca_critical_sections();
		
		bool result = true;


		// Initialize lowest level sub-systems.
		result &= CLogging::Get().initialize();


		CRessourceLoaderFactory::register_ressource_loader("Texture", &CTextureLoader::create);
		CRessourceLoaderFactory::register_ressource_loader("Shader", &CShaderLoader::create);

		return result;
	}


	bool CMintEngine::_load_app_manifiest(maml::CDocument& document, CWindow::SDescription& window_desc, CPhysicsSystem::SDescription& physics_desc, CScene*& initial_scene)
	{
		auto root = document.get_root();

		bool window_data_loaded = false;
		bool scene_data_loaded = false;

		auto window_node = document.find_first_match_in_document("Window");
		auto physics_node = document.find_first_match_in_document("Physics");
		auto package_manifest_node = document.find_first_match_in_document("PackageManifest");
		auto initial_scene_node = document.find_first_match_in_document("InitialScene");

		if(window_node)
		{
			CSerializer::import_bool(&window_desc.m_fullscreen, "Fullscreen", window_node);
			CSerializer::import_bool(&window_desc.m_resizable, "Resizable", window_node);
			CSerializer::import_bool(&window_desc.m_decorated, "Decorated", window_node);
			CSerializer::import_bool(&window_desc.m_visible, "Visible", window_node);
			CSerializer::import_bool(&window_desc.m_minimized, "Minimized", window_node);
			CSerializer::import_bool(&window_desc.m_focus, "Focus", window_node);
			CSerializer::import_bool(&window_desc.m_vsync, "VSYNC", window_node);

			CSerializer::import_vec4(window_desc.m_clearColor, "ClearColor", window_node);
			CSerializer::import_uint(&window_desc.m_width, "Width", window_node);
			CSerializer::import_uint(&window_desc.m_height, "Height", window_node);

			window_data_loaded = true;
		}
		if (physics_node)
		{
			bool use = false;

			CSerializer::import_bool(&use, "Use", physics_node);
			if(use)
			{
				CSerializer::import_bool(&physics_desc.m_debugRender, "DebugRender", physics_node);
				CSerializer::import_float(&physics_desc.m_fixedTimestep, "FixedTimestep", physics_node);
				CSerializer::import_uint(&physics_desc.m_positionIterations, "PositionIterations", physics_node);
				CSerializer::import_uint(&physics_desc.m_velocityIterations, "VelocityIterations", physics_node);
				CSerializer::import_vec2(physics_desc.m_gravity, "Gravity", physics_node);
			
				CPhysicsSystem::set_use_physics(true);
			}
		}
		if (package_manifest_node)
		{
		}
		if (initial_scene_node)
		{
			String scene_name;

			CSerializer::import_string(scene_name, "Scene", initial_scene_node);

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

		// Registry.
		result &= MINT_SCENE_REGISTRY().initialize();

		// Scripting System (entity bound and unbound).

		// Plugin System (initialize plugins).
		CPluginSystem::Get().on_initialization();


		return result;
	}


	bool CMintEngine::_pre_init(CWindow::SDescription& wdesc, CPhysicsSystem::SDescription& pdesc)
	{
		auto& window = get_main_window();

		if (!window.initialize(wdesc)) return false;
		if (CPhysicsSystem::get_use_physics() && !CPhysicsSystem::Get().initialize(pdesc)) return false;


		


		fx::CColor color(wdesc.m_clearColor);


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
			IMintEngine::get_engine()->run_spatial_acceleration_structure();
		}


		return result;
	}


	void CMintEngine::_cleanup_after_terminate()
	{

	}


	void CMintEngine::_pre_terminate()
	{

	}


	void CMintEngine::_terminate()
	{

	}


	void CMintEngine::_post_terminate()
	{
		IMintEngine::delete_cuca_critical_sections();
	}


	void CMintEngine::_on_pre_update()
	{
		CEventSystem::Get().update();
	}


	void CMintEngine::_on_update(f32 dt /*= CTimestep::get_fps()*/)
	{
		
	}


	void CMintEngine::_on_post_update()
	{

	}


	void CMintEngine::_on_late_update()
	{

	}


	void CMintEngine::_begin_frame()
	{
		auto scene = MINT_ACTIVE_SCENE();
		auto camera = scene->get_active_camera();

		fx::CSceneRenderer::Get().on_pre_render(camera);
	}


	void CMintEngine::_frame()
	{
		fx::CSceneRenderer::Get().on_render(MINT_ACTIVE_SCENE());
	}


	void CMintEngine::_end_frame()
	{
		fx::CSceneRenderer::Get().on_post_render();
	}


}