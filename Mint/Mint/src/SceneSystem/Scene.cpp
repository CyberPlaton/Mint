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
		return false;
	}


	bool CScene::export_scene(const String& maml_scene_filepath)
	{
		return false;
	}


	void CScene::transit_to_scene(const String& scene_name)
	{

	}


	fx::ICamera* CScene::get_active_camera()
	{
		return m_camera;
	}


	void CScene::push_camera(fx::ICamera* camera)
	{
		m_camera = camera;
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


}