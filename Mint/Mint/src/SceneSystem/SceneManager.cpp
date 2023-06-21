#include "SceneManager.h"


namespace mint
{
	mint::CSceneManager* CSceneManager::g_CSceneManager = nullptr;


	void CSceneManager::_load_scene_definition(CScene* scene, Vector< CAsset >& scene_assets)
	{
		CFilesystem fs(CFilesystem::get_working_directory());

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

		scene->m_assets = CFilesystem::construct_from(fs.get_current_directory(), assets);
		scene->m_persistence = CFilesystem::construct_from(fs.get_current_directory(), entities);
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


			CFilesystem fpath = fs;
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
		CFilesystem fs(asset.get_asset_source_path());

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

		// Populate spatial acceleration structure with entities.
		// Static entities are submitted once, dynamic entities are submitted each starting with this.
		m_current->submit_dynamic_entities();
		m_current->submit_static_entities();
	}


	void CSceneManager::unload_scene(CScene* scene)
	{
		scene->on_before_unload();

		if(scene->is_persistent())
		{
			if (scene->export_scene(scene->get_scene_persistence_path().as_string()))
			{
				MINT_LOG_INFO("[{:.4f}][CSceneManager::unload_scene] Success exporting scene: \"{}\"", MINT_APP_TIME, scene->get_scene_persistence_path().as_string());
			}
			else
			{
				MINT_LOG_CRITICAL("[{:.4f}][CSceneManager::unload_scene] Failed exporting scene: \"{}\"", MINT_APP_TIME, scene->get_scene_persistence_path().as_string());
			}
		}

		scene->on_unload();


		// Unloading the scene basically means reset the system to default state.
		CEventSystem::Get().reset();

		animation::CAnimationSystem::Get().reset();

		scripting::CScriptEngine::Get().reset();

		scripting::CBehaviorEngine::Get().reset();

		fx::CMaterialManager::Get().reset();

		CPhysicsSystem::Get().reset();

		CSAS::Get().reset();

		MINT_SCENE_REGISTRY()->reset(scene->get_entities());

		CShaderManager::Get().reset();

		CTextureManager::Get().reset();
	}


	void CSceneManager::load_scene(CScene* scene)
	{
		IScene::set_active_scene(scene);

		Vector< CAsset > assets;

		_load_common_asset_ressources();

		_load_scene_definition(scene, assets);

		scene->on_before_load();
		
		for(auto& asset: assets)
		{
			_load_scene_asset_ressources(scene, asset);
		}


		if(scene->is_persistent())
		{
			if (scene->import_scene(scene->get_scene_persistence_path().as_string()))
			{
				MINT_LOG_INFO("[{:.4f}][CSceneManager::load_scene] Success importing scene: \"{}\"", MINT_APP_TIME, scene->get_scene_persistence_path().as_string());
			}
			else
			{
				MINT_LOG_CRITICAL("[{:.4f}][CSceneManager::load_scene] Failed importing scene: \"{}\"", MINT_APP_TIME, scene->get_scene_persistence_path().as_string());
			}
		}

		scene->on_load();
	}


	bool CSceneManager::is_transitioning()
	{
		return m_transition;
	}


	void CSceneManager::_load_common_asset_ressources()
	{
		Vector< CAsset > scene_assets;

		CFilesystem fs(CFilesystem::get_working_directory());

		fs.forward("Scenes");
		
		maml::CDocument document;

		auto root = CSerializer::load_maml_document(CFilesystem::construct_from(fs.get_current_directory().as_string(), "common.maml").as_string(), document);

		MINT_ASSERT(root != nullptr, "Failed loading common scene ressources!");


		auto asset_node = document.find_first_match_in_document("asset");

		String entities, path;

		CSerializer::import_string(path, "assets", asset_node);

		auto ressource_declaration_node = document.get_node_children(asset_node);

		for (auto& node : ressource_declaration_node)
		{
			CAsset ressource;

			String folder, extension, type;
			bool recurse;

			CSerializer::import_string(folder, "folder", node);
			CSerializer::import_string(extension, "extension", node);
			CSerializer::import_string(type, "type", node);
			CSerializer::import_bool(&recurse, "recursive", node);


			CFilesystem fpath = fs;
			fpath.append_path(path);
			fpath.append_path(folder);

			ressource.set_asset_source_path(fpath.get_current_directory().as_string());
			ressource.set_ressource_type(type);
			ressource.set_asset_extension(extension);

			ressource.write_bool("recursive", recurse);


			scene_assets.push_back(ressource);
		}


		for(auto& asset: scene_assets)
		{
			_load_common_asset_ressource(asset);
		}
	}


	void CSceneManager::_load_common_asset_ressource(CAsset& asset)
	{
		CFilesystem fs(asset.get_asset_source_path());

		String extension, type;
		extension = asset.get_asset_extension();
		type = asset.get_ressource_type();


		auto files = fs.get_all_files_in_current_dir();
		for (auto& file : files)
		{
			if (file.has_extension(extension))
			{
				maml::CDocument document;

				auto root = CSerializer::load_maml_document(file.as_string(), document);

				if (root)
				{
					auto asset_node = document.find_first_match_in_node(root, "asset");

					auto ressource_loader = CRessourceLoaderFactory::create_ressource_loader(type);

					if (ressource_loader)
					{
						CAsset def = ressource_loader->load_asset(fs.get_current_directory().as_string(), type, asset_node);


						if (!ressource_loader->load_ressource(type, def))
						{
							MINT_LOG_ERROR("[{:.4f}][CSceneManager::_load_scene_asset_ressources] Failed loading asset \"{}\" at \"{}\" of type \"{}\" for scene \"{}\"!",
								MINT_APP_TIME, def.get_asset_name(), def.get_asset_source_path(), type, MINT_ACTIVE_SCENE()->get_scene_name());
						}
					}
				}
			}
		}


		if (asset.read_bool("recursive"))
		{
			auto dirs = fs.get_all_directories_in_current_dir();
			for (auto& dir : dirs)
			{
				CPath path = fs.get_current_directory() /= dir;

				CAsset recursive_asset_decl = asset;

				recursive_asset_decl.set_asset_source_path(path.as_string());

				_load_common_asset_ressource(recursive_asset_decl);
			}
		}
	}


}