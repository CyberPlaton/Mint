#ifndef _MINT_ISCENE_H_
#define _MINT_ISCENE_H_


#include "Common/Common.h"

#include "../../Graphics/Common/Camera.h"
#include "Registry.h"
#include "Utility/FileSystem/Filesystem.h"


namespace mint
{
	class CSceneManager;

	class IScene
	{
		friend class CSceneManager;
	public:
		typedef bool (*ComponentExporterFunction)(entt::entity, entt::id_type, const entt::registry&, maml::SNode*);
		typedef bool (*ComponentImporterFunction)(entt::entity, entt::id_type, entt::registry&, maml::SNode*);

	public:
		static IScene* get_active_scene();
		static void set_active_scene(IScene* scene);
		
		template < class T >
		static void register_component_exporter(ComponentExporterFunction function);

		template < class T >
		static void register_component_importer(ComponentImporterFunction function);

		static ComponentExporterFunction get_component_exporter(entt::id_type id);

		static ComponentImporterFunction get_component_importer(entt::id_type id);

		static bool does_component_exporter_exist(entt::id_type id);

		static bool does_component_importer_exist(entt::id_type id);



		virtual void on_update(f32) = 0;

		virtual void on_ui_render(f32) = 0;

		virtual bool on_before_load() = 0;

		virtual bool on_load() = 0;

		virtual void on_before_unload() = 0;

		virtual void on_unload() = 0;

		virtual String get_scene_name() = 0;

		virtual bool is_ready() = 0;



		virtual void add_entity(entt::entity) = 0;

		virtual void remove_entity(entt::entity) = 0;

		virtual Vector< entt::entity > get_entities() = 0;

		virtual Vector< entt::entity > get_dynamic_entities() = 0;

		virtual Vector< entt::entity > get_static_entities() = 0;

		virtual CRegistry& get_registry() = 0;



		virtual bool import_scene(const String&) = 0;

		virtual bool export_scene(const String&) = 0;

		virtual bool is_persistent() = 0;


		virtual void transit_to_scene(const String&) = 0;


		virtual CPath get_scene_assets_path() = 0;

		virtual CPath get_scene_persistence_path() = 0;

		virtual CPath get_scene_relative_path() = 0;

		virtual CPath get_scene_full_path() = 0;

	private:
		static IScene* s_activeScene;

		static CMap< ComponentExporterFunction > s_componentExporter;

		static CMap< ComponentImporterFunction > s_componentImporter;


	protected:
		virtual bool import_entity(maml::SNode*) = 0;

		virtual bool export_entity(entt::entity, maml::SNode*) = 0;


	};


	template < class T >
	void mint::IScene::register_component_importer(mint::IScene::ComponentImporterFunction function)
	{
		auto h = entt::type_id< T >().hash();

		s_componentImporter.add(h, function);
	}


	template < class T >
	void mint::IScene::register_component_exporter(mint::IScene::ComponentExporterFunction function)
	{
		auto h = entt::type_id< T >().hash();

		s_componentExporter.add(h, function);
	}


}


#define MINT_SCENE_REGISTRY() \
mint::IScene::get_active_scene()->get_registry()


#define MINT_ACTIVE_SCENE() \
mint::IScene::get_active_scene()

#endif