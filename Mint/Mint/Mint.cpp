#include "Mint.h"

namespace mint
{
	
	mint::CMintEngine* CMintEngine::s_CMintEngine = nullptr;


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


	mint::fx::SViewport& CMintEngine::get_main_viewport()
	{
		return m_mainViewport;
	}


	bool CMintEngine::_prepare_for_init()
	{
		INITIALIZE_CRITICAL_SECTION(CUCA::m_identifierCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_hierarchyCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_transformCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_rigidBodyCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_spriteCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_animatedSpriteCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_scriptCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_dirtyFlagCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);

		bool result = true;

		result &= CLogging::Get().initialize();

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

		// Registry.
		result &= MINT_SCENE_REGISTRY().initialize();

		// Scripting System (entity bound and unbound).

		// Plugin System (initialize plugins).
		CPluginSystem::Get().on_initialization();


		return result;
	}


	bool CMintEngine::_pre_init(CWindow::SDescription& wdesc, CPhysicsSystem::SDescription& pdesc)
	{
		if (!m_mainWindow.initialize(wdesc)) return false;
		if (CPhysicsSystem::get_use_physics() && !CPhysicsSystem::Get().initialize(pdesc)) return false;


		// Initialize BGFX.
		bgfx::renderFrame();

		bgfx::Init bgfxInit;
		bgfxInit.resolution.width = wdesc.m_width;
		bgfxInit.resolution.height = wdesc.m_height;

		if (wdesc.m_vsync)
		{
			bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
		}
		else
		{
			bgfxInit.resolution.reset = BGFX_RESET_NONE;
		}

		bgfxInit.platformData.nwh = m_mainWindow.get_native_handle< void* >();

		if (!bgfx::init(bgfxInit))
		{
			m_mainWindow.terminate();
			m_running = false;
			return false;
		}

		m_mainViewport.m_top = 0;
		m_mainViewport.m_left = 0;
		m_mainViewport.m_right = wdesc.m_width;
		m_mainViewport.m_bottom = wdesc.m_height;
		m_mainViewport.m_viewIdentifier = 0;

		bgfx::setViewClear(m_mainViewport.m_viewIdentifier, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
			mint::algorithm::convert_from_argb(wdesc.m_clearColor), 1.0f, 0);

		bgfx::setViewRect(m_mainViewport.m_viewIdentifier, 0, 0, bgfx::BackbufferRatio::Equal);


		// Initialize medium level engine systems.
		bool result = true;

		// Event System.
		result &= CEventSystem::Get().initialize();
		if(result)
		{
			IMintEngine::get_engine()->run_event_system();
		}

		// Shader manager.

		// Texture manager.

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
		DELETE_CRITICAL_SECTION(CUCA::m_identifierCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_hierarchyCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_transformCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_rigidBodyCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_spriteCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_animatedSpriteCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_scriptCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_dirtyFlagCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);

	}


	void CMintEngine::_on_pre_update()
	{

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
		fx::CSceneRenderer::Get().begin(get_main_viewport());
	}


	void CMintEngine::_frame()
	{
		fx::CSceneRenderer::Get().render(get_main_viewport(), MINT_ACTIVE_SCENE());
	}


	void CMintEngine::_end_frame()
	{
		fx::CSceneRenderer::Get().end();
	}


}