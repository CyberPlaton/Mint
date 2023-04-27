#include "Main.h"


#include "Scenes/MainScene.h"

/// @brief 
void register_scenes()
{
	using namespace mint;

	MINT_REGISTER_SCENE(CMainScene);	
}

MINT_MAIN();