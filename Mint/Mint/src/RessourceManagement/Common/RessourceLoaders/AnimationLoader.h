#ifndef _MINT_ANIMATION_LOADER_H_
#define _MINT_ANIMATION_LOADER_H_


#include "IRessourceLoader.h"
#include "AnimationSystem/AnimationSystem.h"


namespace mint
{
	class CAnimationLoader : public IRessourceLoader
	{
	public:
		MINT_DEFINE_RESSOURCE_LOADER_CTOR(CAnimationLoader);

		bool load_ressource(const String& ressource_type, CAsset& asset) override final;

		CAsset load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node) override final;
	};
}


#endif