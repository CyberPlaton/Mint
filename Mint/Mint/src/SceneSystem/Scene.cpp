#include "Scene.h"

namespace mint
{


	CScene::CScene()
	{
	}


	CScene::~CScene()
	{
		m_entities.clear();
	}


	void CScene::add_entity(entt::entity entity)
	{
		m_entities.push_back(entity);
	}


	void CScene::remove_entity(entt::entity entity)
	{
		mint::algorithm::vector_erase(m_entities, entity);
	}

	mint::Vector< entt::entity > CScene::get_entities()
	{
		return m_entities;
	}


	mint::Vector< entt::entity > CScene::get_dynamic_entities()
	{
		Vector< entt::entity > entities;

		for(auto& entity: get_entities())
		{
			if(MINT_SCENE_REGISTRY().has_component< mint::component::SSprite >(entity) &&
			   MINT_SCENE_REGISTRY().has_component< mint::component::SDynamicGameobject >(entity))
			{
				entities.push_back(entity);
			}
		}

		return entities;
	}


	mint::Vector< entt::entity > CScene::get_static_entities()
	{
		Vector< entt::entity > entities;

		for (auto& entity : get_entities())
		{
			if (MINT_SCENE_REGISTRY().has_component< mint::component::SSprite >(entity) &&
				!MINT_SCENE_REGISTRY().has_component< mint::component::SDynamicGameobject >(entity))
			{
				entities.push_back(entity);
			}
		}

		return entities;
	}


	mint::CRegistry& CScene::get_registry()
	{
		return m_registry;
	}


	bool CScene::import_scene(const String& maml_scene_filepath)
	{
		maml::CDocument document;

		auto root = CSerializer::load_maml_document(maml_scene_filepath, document);

		MINT_ASSERT(root != nullptr, "Failed loading scene persistence file!");

		if (root == nullptr) return false;

		// Import scene data.

		// Import scene entities.
		auto entities_node = document.find_first_match_in_document("entities");
		
		// Get all entity entries.
		auto& entity_entries = maml::CDocument::get_all_node_properties(entities_node);

		// Go through all entries and load them as entities.
		bool result;
		for (auto& entity_path : entity_entries)
		{
			CFilesystem fs(CFilesystem::get_working_directory());

			if (fs.forward(entity_path.cast< String >()))
			{
				maml::CDocument entity_document;

				auto entity_root = CSerializer::load_maml_document(fs.get_current_directory().as_string(), entity_document);

				if (entity_root)
				{
					result = import_entity(entity_root);
				}
				
				MINT_ASSERT(entity_root != nullptr, "Invalid operation. Entity could not be loaded!");
				MINT_ASSERT(result == true, "Invalid operation. Entity could not be loaded!");
			}
		}

		return true;
	}


	bool CScene::export_scene(const String& maml_scene_filepath)
	{
		maml::CDocument document;

		auto root = document.get_root();

		MINT_ASSERT(root != nullptr, "Failed creating MAML scene document!");

		// Export scene data.

		// Export entities.
		auto entities_node = document.create_node("entities", root);
		bool result;
		String entity_file_id;
		for(const auto& entity : m_entities)
		{
			// Try exporting the entity file to e.g. "{%SceneName}/entities/Entity_01.entity".
			maml::CDocument entityDocument(MAML_DOCUMENT_SIZE_SMALL);

			auto entity_node = entityDocument.create_node("entity");

			result = export_entity(entity, entity_node);

			MINT_ASSERT(result == true, "Invalid operation. Failed exporting entity components!");


			if (result)
			{
				// Construct path where to export the entity to.
				entity_file_id = "Entity_" + std::to_string(CUCA::identifier_get_identifier(entity));

				CFilesystem fs(get_scene_assets_path());

				if (fs.forward("entities"))
				{
					fs.append_path(entity_file_id + ".entity");

					result = entityDocument.save_document(fs.get_current_directory().as_string());

					if (!result)
					{
						MINT_LOG_ERROR("[{:.4f}][CScene::export_scene] Failed exporting entity file to: \"{}\"", MINT_APP_TIME, fs.get_current_directory().as_string());
					}
				}
			}


			// Set the entity file path as value for entity node if and only if we successfully exported the file.
			if (result)
			{
				document.add_property_to_node(entities_node, entity_file_id, "entities/" + entity_file_id);
			}

			MINT_ASSERT(result == true, "Invalid operation. Failed exporting entity!");
		}


		if(!document.save_document(maml_scene_filepath))
		{
			MINT_LOG_ERROR("[{:.4f}][CScene::export_scene] Failed exporting scene: \"{}\"", MINT_APP_TIME, maml_scene_filepath);
			return false;
		}

		return true;
	}


	void CScene::transit_to_scene(const String& scene_name)
	{
		auto event = new SEvent("Event_SceneTransit", scene_name);

		CEventSystem::Get().queue_event(event);
	}


	mint::CPath CScene::get_scene_assets_path()
	{
		return m_assets;
	}


	mint::CPath CScene::get_scene_persistence_path()
	{
		return m_persistence;
	}


	mint::CPath CScene::get_scene_relative_path()
	{
		return m_relative;
	}


	mint::CPath CScene::get_scene_full_path()
	{
		return m_full;
	}


	bool CScene::import_entity(maml::SNode* maml_node)
	{
		auto& document = *maml_node->m_document;

		entt::entity entity = entt::null;

		auto identifier_node = document.find_first_match_in_node(maml_node, "identifier");

		MINT_ASSERT(identifier_node != nullptr, "Failed importing entity. Entity does not have a SIdentifier component!");

		auto id = SCAST(u64, -1);

		CSerializer::import_uint(&id, "id", identifier_node, -1);

		entity = m_registry.create_entity(id);

		if (entity == entt::null) return false;


		const auto& registry = m_registry.m_registry;

		bool result = true;

		for (auto&& curr : registry.storage())
		{
			if (auto& storage = curr.second; storage.contains(entity))
			{
				// Ignore SIdentifier component, as it was already imported.
				if (entt::type_id< mint::component::SIdentifier >().hash() == curr.first) continue;

				if (IScene::does_component_importer_exist(curr.first)) // Import only components for which we registered importers.
				{
					result &= IScene::get_component_importer(curr.first)(entity, curr.first, registry, maml_node);
				}
			}
		}

		return result;
	}


	bool CScene::export_entity(entt::entity entity, maml::SNode* maml_node)
	{
		auto& registry = m_registry.m_registry;

		bool result = true;

		for(auto&& curr: registry.storage())
		{
			if(auto& storage = curr.second; storage.contains(entity))
			{
				if (IScene::does_component_exporter_exist(curr.first)) // Export only components for which we registered an exporter.
				{
					result &= IScene::get_component_exporter(curr.first)(entity, curr.first, registry, maml_node);
				}
			}
		}

		return result;
	}


}