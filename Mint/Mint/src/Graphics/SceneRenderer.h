#ifndef _MINTFX_SCENE_RENDERER_H_
#define _MINTFX_SCENE_RENDERER_H_


#include "Common/Common.h"
#include "SceneSystem/Common/Camera.h"
#include "SceneSystem/Common/IScene.h"
#include "Components/CUCA.h"
#include "Common/Timestep.h"
#include "Common/SpriteBatch.h"


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

		CSpriteBatch m_spriteBatch;
	};
}


#endif