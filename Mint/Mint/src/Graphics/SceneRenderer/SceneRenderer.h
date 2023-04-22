#ifndef _MINTFX_SCENE_RENDERER_H_
#define _MINTFX_SCENE_RENDERER_H_


#include "Common/Rectangle.h"


#include "../../SceneSystem/Common/IScene.h"
#include "../../Components/CUCA.h"
#include "../../RessourceManagement/TextureManager.h"
#include "../Shaders/MaterialManager.h"


namespace mint::fx
{

	class CSceneRenderer
	{
	public:
		STATIC_GET(CSceneRenderer, s_CSceneRenderer);

		bool initialize();

		void terminate();


		void on_pre_render(ICamera* render_camera);

		void on_render(mint::IScene* scene);

		void on_render(Vector< entt::entity >& entities);

		void on_post_render();

		void on_resize(ICamera* render_camera);


	private:
		static CSceneRenderer* s_CSceneRenderer;

		ICamera* m_renderingCamera = nullptr;

	private:

	};

}


#endif