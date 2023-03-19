#ifndef _MINT_SCENE_MANAGER_H_
#define _MINT_SCENE_MANAGER_H_


#include "Scene.h"
#include "RessourceManagement/Common/Asset.h"
#include "Common/SceneFactory.h"
#include "RessourceManagement/Common/RessourceLoaders/RessourceLoaderFactory.h"
#include "Utility/Logging/Logging.h"
#include "Utility/EventSystem/EventSystem.h"
#include "SAS.h"


namespace mint
{
	class CSceneManager
	{
	public:
		STATIC_GET(CSceneManager, g_CSceneManager);

		bool initialize();

		void terminate();


		
		void update();

		void set_current_scene(const String& scene_name);

		CScene* get_current_scene();

		void set_initial_scene(CScene* scene);



		void unload_scene(CScene* scene);

		void load_scene(CScene* scene);


	private:
		static CSceneManager* g_CSceneManager;

		CScene* m_current;

		bool m_transition;

		String m_destination;

		CMap< String > m_scenePaths;


	private:
		void _load_scene_definition(CScene* scene, Vector< CAsset >& scene_assets);

		void _load_scene_asset_ressources(CScene* scene, CAsset& asset);
	};

}



#define MINT_REGISTER_SCENE(class_name) \
CSceneFactory::register_scene(STRING(class_name), &class_name::create)


#define MINT_DEFINE_SCENE_CTOR(class_name) \
static mint::CScene* create() {return new class_name();} \
mint::String get_scene_name() override final {return STRING(class_name);} \
bool m_ready = false; \
bool is_ready() override final { return m_ready; }

#endif