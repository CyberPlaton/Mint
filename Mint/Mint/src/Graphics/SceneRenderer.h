#ifndef _MINTFX_SCENE_RENDERER_H_
#define _MINTFX_SCENE_RENDERER_H_


#include "SceneSystem/Common/Camera.h"
#include "SceneSystem/Common/IScene.h"


namespace mint::fx
{

	class CSceneRenderer
	{
	public:
		struct SDescription
		{
			bgfx::RendererType::Enum m_api;

			void* m_nativeWindowHandle;

			u32 m_width;

			u32 m_height;

			bool m_vsync;
		};


	public:

		bool initialize(SDescription& desc);

		void terminate();


		void begin(SViewport& viewport);

		void render(SViewport& viewport, mint::IScene* scene = mint::IScene::get_active_scene());

		void end();




	private:

	};
}


#endif