#ifndef _MINT_SCENE_FACTORY_H_
#define _MINT_SCENE_FACTORY_H_


#include "Common/Algorithm.h"
#include "Utility/STL/Map.h"


namespace mint
{
	class CScene;

	class CSceneFactory
	{
		typedef CScene* (*FactoryType)();
	public:

		static CScene* create_scene(const String& scene_name);

		static void register_scene(const String& scene_name, FactoryType scene_class_constructor);


	private:
		static CMap< FactoryType > m_scenes;
	};
}


#endif