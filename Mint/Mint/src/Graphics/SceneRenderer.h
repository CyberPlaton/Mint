#ifndef _MINTFX_SCENE_RENDERER_H_
#define _MINTFX_SCENE_RENDERER_H_


#include "SceneSystem/Common/Camera.h"
#include "SceneSystem/Common/IScene.h"


namespace mint::fx
{

	class CSceneRenderer
	{
	public:
		STATIC_GET(CSceneRenderer, s_CSceneRenderer);

		bool initialize();

		void terminate();


		void begin(SViewport& viewport);

		void render(SViewport& viewport, mint::IScene* scene = mint::IScene::get_active_scene());

		void end();


	private:
		static CSceneRenderer* s_CSceneRenderer;


	};
}


#endif