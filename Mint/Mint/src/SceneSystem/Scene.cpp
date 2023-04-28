#include "Scene.h"

namespace mint
{


	CScene::CScene()
	{

	}


	CScene::~CScene()
	{

	}


	void CScene::add_entity(entt::entity entity)
	{
		m_entities.push_back(entity);
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
		bool result;
		for(auto& node : maml::CDocument::get_node_children(entities_node))
		{
			result = import_entity(node);

			MINT_ASSERT(result == false, "Failed importing entity!");
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
		for(const auto& entity : m_entities)
		{
			auto entity_node = document.create_node("entity", entities_node);

			result = export_entity(entity, entity_node);

			MINT_ASSERT(result == false, "Failed exporting entity!");
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

	}


	fx::ICamera* CScene::get_active_camera()
	{
		return mint::algorithm::vector_get_last_element_as< fx::ICamera* >(m_cameraStack);
	}


	void CScene::push_camera(fx::ICamera* camera)
	{
		mint::algorithm::vector_push_back(m_cameraStack, camera);
	}


	void CScene::pop_camera()
	{
		mint::algorithm::vector_erase_last(m_cameraStack);
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


				result &= IScene::get_component_importer(curr.first)(entity, curr.first, registry, maml_node);
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
				result &= IScene::get_component_exporter(curr.first)(entity, curr.first, registry, maml_node);
			}
		}

		return result;
	}


}