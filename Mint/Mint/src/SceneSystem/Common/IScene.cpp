#include "IScene.h"


namespace mint
{
	mint::IScene* IScene::s_activeScene = nullptr;
	mint::CMap< mint::IScene::ComponentExporterFunction > IScene::s_componentExporter;
	mint::CMap< mint::IScene::ComponentImporterFunction > IScene::s_componentImporter;


	mint::IScene* IScene::get_active_scene()
	{
		return IScene::s_activeScene;
	}


	void IScene::set_active_scene(IScene* scene)
	{
		s_activeScene = scene;
	}


	bool IScene::does_component_exporter_exist(entt::id_type id)
	{
		return s_componentExporter.lookup(id);
	}


	bool IScene::does_component_importer_exist(entt::id_type id)
	{
		return s_componentImporter.lookup(id);
	}


	void IScene::get_all_component_importer_hash_value(Vector< u64 >& v)
	{
		for (auto i : s_componentImporter.get_indices())
		{
			mint::algorithm::vector_push_back(v, i.first);
		}
	}


	void IScene::get_all_component_exporter_hash_value(Vector< u64 >& v)
	{
		for (auto i : s_componentExporter.get_indices())
		{
			mint::algorithm::vector_push_back(v, i.first);
		}
	}

	mint::IScene::ComponentImporterFunction mint::IScene::get_component_importer(entt::id_type id)
	{
		return s_componentImporter.get(id);
	}


	mint::IScene::ComponentExporterFunction mint::IScene::get_component_exporter(entt::id_type id)
	{
		return s_componentExporter.get(id);
	}
}