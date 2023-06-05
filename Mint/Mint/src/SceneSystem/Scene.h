#ifndef _MINT_SCENE_H_
#define _MINT_SCENE_H_


#include "Common/IScene.h"
#include "Components/Components.h"
#include "Utility/EventSystem/EventSystem.h"
#include "Components/CUCA.h"

namespace mint
{
	class CSceneManager;

	class CScene : public IScene
	{
		friend class CSceneManager;
	public:
		CScene();
		virtual ~CScene();


		virtual void on_update(f32 dt = 0.0f) override {}

		virtual void on_ui_render(f32 dt = 0.0f) override {};

		virtual bool on_before_load() override { return false; };

		virtual bool on_load() override { return false; };

		virtual void on_before_unload() override {};

		virtual void on_unload() override {};



		void add_entity(entt::entity entity) override final;

		void remove_entity(entt::entity entity) override final;

		Vector< entt::entity > get_entities() override final;

		Vector< entt::entity > get_dynamic_entities() override final;

		Vector< entt::entity > get_static_entities() override final;

		CRegistry& get_registry() override final;



		bool import_scene(const String& maml_scene_filepath) override final;

		bool export_scene(const String& maml_scene_filepath) override final;

		virtual bool is_persistent() { return false; }


		void transit_to_scene(const String& scene_name) override final;



		CPath get_scene_assets_path() override final;

		CPath get_scene_persistence_path() override final;

		CPath get_scene_relative_path() override final;

		CPath get_scene_full_path() override final;


	protected:
		Vector< entt::entity > m_entities;

		CRegistry m_registry;

		CPath m_assets;
		CPath m_persistence;
		CPath m_relative;
		CPath m_full;

	private:
		

	protected:
		bool import_entity(maml::SNode* maml_node) override final;

		bool export_entity(entt::entity entity, maml::SNode* maml_node) override final;
	};
}

#endif