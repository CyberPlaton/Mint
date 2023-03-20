#include "IScene.h"


namespace mint
{
	mint::IScene* IScene::s_activeScene = nullptr;


	mint::IScene* IScene::get_active_scene()
	{
		return IScene::s_activeScene;
	}


	void IScene::set_active_scene(IScene* scene)
	{
		s_activeScene = scene;
	}


}