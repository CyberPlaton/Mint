#include "SceneManager.h"


namespace mint
{
	mint::CSceneManager* CSceneManager::g_CSceneManager = nullptr;


	void CSceneManager::_load_scene_definition(CScene* scene, Vector< CAsset >& scene_assets)
	{
		CFileystem fs(CFileystem::get_working_directory());

		fs.forward("Scenes");
		fs.forward(scene->get_scene_name());

		auto scene_definition_path = fs.get_full_path_of_first_file_by_extension(".scene");


		maml::CDocument document;

		auto root = CSerializer::load_maml_document(scene_definition_path.as_string(), document);

		MINT_ASSERT(root != nullptr, "Failed reading scene definition!");


		auto asset_node = document.find_first_match_in_document("asset");

		String entities, assets, path;

		CSerializer::import_string(entities, "entities", asset_node);
		CSerializer::import_string(assets, "assets", asset_node);
		CSerializer::import_string(path, "path", asset_node);

		scene->m_assets = CFileystem::construct_from(fs.get_current_directory(), assets);
		scene->m_persistence = CFileystem::construct_from(fs.get_current_directory(), entities);
		scene->m_relative = path;
		scene->m_full = fs.get_current_directory();

		
		auto ressource_declaration_node = document.get_node_children(asset_node);

		for(auto& node: ressource_declaration_node)
		{
			CAsset ressource;

			String folder, extension, type;
			bool recurse;

			CSerializer::import_string(folder, "folder", node);
			CSerializer::import_string(extension, "extension", node);
			CSerializer::import_string(path, "path", node);
			CSerializer::import_string(type, "type", node);
			CSerializer::import_bool(&recurse, "recursive", node);


			CFileystem fpath = fs;
			fpath.append_path(assets);
			fpath.append_path(folder);

			ressource.set_asset_source_path(fpath.get_current_directory().as_string());
			ressource.set_ressource_type(type);
			ressource.set_asset_extension(extension);

			ressource.write_bool("recursive", recurse);


			scene_assets.push_back(ressource);
		}
	}


	void CSceneManager::_load_scene_asset_ressources(CScene* scene, CAsset& asset)
	{
		CFileystem fs(asset.get_asset_source_path());

		String extension, type;
		extension = asset.get_asset_extension();
		type = asset.get_ressource_type();


		auto files = fs.get_all_files_in_current_dir();
		for(auto& file: files)
		{
			if(file.has_extension(extension))
			{
				maml::CDocument document;

				auto root = CSerializer::load_maml_document(file.as_string(), document);

				if(root)
				{
					auto asset_node = document.find_first_match_in_node(root, "asset");

					auto ressource_loader = CRessourceLoaderFactory::create_ressource_loader(type);

					if(ressource_loader)
					{
						CAsset def = ressource_loader->load_asset(fs.get_current_directory().as_string(), type, asset_node);


						if(!ressource_loader->load_ressource(type, def))
						{
							MINT_LOG_ERROR("[{:.4f}][CSceneManager::_load_scene_asset_ressources] Failed loading asset \"{}\" at \"{}\" of type \"{}\" for scene \"{}\"!", 
											MINT_APP_TIME, def.get_asset_name(), def.get_asset_source_path(), type, MINT_ACTIVE_SCENE()->get_scene_name());
						}
					}
				}
			}
		}


		if(asset.read_bool("recursive"))
		{
			auto dirs = fs.get_all_directories_in_current_dir();
			for(auto& dir: dirs)
			{
				CPath path = fs.get_current_directory() /= dir;

				CAsset recursive_asset_decl = asset;

				recursive_asset_decl.set_asset_source_path(path.as_string());

				_load_scene_asset_ressources(scene, recursive_asset_decl);
			}
		}
	}


	bool CSceneManager::initialize()
	{
		MINT_REGISTER_EVENT_LISTENER(SSceneChangeEventDelegate);
		return true;
	}


	void CSceneManager::terminate()
	{
		delete m_current;
		m_current = nullptr;
		IScene::s_activeScene = nullptr;

		m_transition = false;
		m_destination = "";
	}


	void CSceneManager::update()
	{
		if(m_transition)
		{
			unload_scene(m_current);

			delete m_current;
			m_current = nullptr;
			IScene::s_activeScene = nullptr;


			m_current = CSceneFactory::create_scene(m_destination);

			load_scene(m_current);


			m_transition = false;
			m_destination = "";
		}
	}


	void CSceneManager::set_current_scene(const String& scene_name)
	{
		m_transition = true;
		m_destination = scene_name;
	}


	mint::CScene* CSceneManager::get_current_scene()
	{
		return m_current;
	}


	void CSceneManager::set_initial_scene(CScene* scene)
	{
		m_current = scene;

		load_scene(scene);
	}


	void CSceneManager::unload_scene(CScene* scene)
	{
		scene->on_before_unload();

		if(scene->is_persistent())
		{
			scene->export_scene(" TODO ");
		}

		scene->on_unload();


		// Unloading the scene basically means reset the system to default state.
		CEventSystem::Get().reset();

		scripting::CScriptEngine::Get().reset();

		scripting::CBehaviorEngine::Get().reset();

		fx::CMaterialManager::Get().reset();

		if (CPhysicsSystem::get_use_physics()) CPhysicsSystem::Get().reset();

		CSAS::Get().reset();

		MINT_SCENE_REGISTRY().reset(scene->get_entities());

		CShaderManager::Get().reset();

		CTextureManager::Get().reset();
	}


	void CSceneManager::load_scene(CScene* scene)
	{
		IScene::set_active_scene(scene);

		Vector< CAsset > assets;

		_load_scene_definition(scene, assets);

		scene->on_before_load();
		
		for(auto& asset: assets)
		{
			_load_scene_asset_ressources(scene, asset);
		}


		if(scene->is_persistent())
		{
			scene->import_scene(" TODO ");
		}

		scene->on_load();


		// Initialize the spatial acceleration structure. 
		CSAS::Get().submit_scene_dynamic_entities(scene->get_dynamic_entities());

		CSAS::Get().submit_scene_static_entities(scene->get_static_entities());
	}


	bool CSceneManager::is_transitioning()
	{
		return m_transition;
	}


}