#include "SceneFactory.h"

namespace mint
{

	mint::CMap< mint::CSceneFactory::FactoryType > CSceneFactory::m_scenes;


	mint::CScene* CSceneFactory::create_scene(const String& scene_name)
	{
		auto h = mint::algorithm::djb_hash(scene_name);

		if(m_scenes.lookup(h))
		{
			return m_scenes.get(h)();
		}

		return nullptr;
	}


	void CSceneFactory::register_scene(const String& scene_name, FactoryType scene_class_constructor)
	{
		auto h = mint::algorithm::djb_hash(scene_name);

		m_scenes.add(h, scene_class_constructor);
	}

}