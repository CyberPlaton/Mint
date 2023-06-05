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
		maml::CDocument document(MAML_DOCUMENT_SIZE_HUGE);

		auto root = CSerializer::load_maml_document(maml_scene_filepath, document);

		MINT_ASSERT(root != nullptr, "Failed loading scene persistence file!");

		if (root == nullptr) return false;

		// Import scene data.

		// Import scene entities.
		auto entities_node = document.find_first_match_in_document("entities");
		MINT_ASSERT(entities_node != nullptr, "Invalid operation. Persistence file does not contain entities entry!");

		// Go through all entity nodes and load each entity.
		auto entity_nodes = document.get_node_children(entities_node);
		bool result;
		for (auto& entity_node : entity_nodes)
		{
			result = import_entity(entity_node);
			MINT_ASSERT(result == true, "Invalid operation. Entity could not be loaded!");

			if (!result)
			{
				MINT_LOG_ERROR("[{:.4f}][CScene::import_scene] Failed exporting entity!", MINT_APP_TIME);
			}
		}

		return true;
	}


	bool CScene::export_scene(const String& maml_scene_filepath)
	{
		maml::CDocument document(MAML_DOCUMENT_SIZE_HUGE);

		auto root = document.get_root();

		MINT_ASSERT(root != nullptr, "Failed creating MAML scene document!");

		// Export scene data.

		// Export entities.
		auto entities_node = document.create_node("entities", root);
		bool result;
		String entity_file_id;
		for(const auto& entity : m_entities)
		{
			// Create a new node for the entity to export to.
			auto entity_node = document.create_node("entity", entities_node);

			if (entity_node == nullptr)
			{
				// No more room left. Inform for Debug purposes and stop exporting.
				MINT_ASSERT(entity_node != nullptr, "Invalid operation. Failed exporting entity!");
				MINT_LOG_ERROR("[{:.4f}][CScene::export_scene] Failed exporting entity, as theres no more room left in the persistence file!", MINT_APP_TIME);
				break;
			}


			// Try exporting.
			result = export_entity(entity, entity_node);

			MINT_ASSERT(result == true, "Invalid operation. Failed exporting entity components!");

			if (!result)
			{
				MINT_LOG_ERROR("[{:.4f}][CScene::export_scene] Failed exporting entity: \"{}\"", MINT_APP_TIME, CUCA::identifier_get_debug_name(entity));
			}
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

		auto entity_node = document.find_first_match_in_document("entity");

		auto id = maml::CDocument::get_uint_property(entity_node, "id", -1);

		entity = m_registry.create_entity(id);

		if (entity == entt::null) return false;


		auto& registry = m_registry.m_registry;

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

		if (result) add_entity(entity);

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