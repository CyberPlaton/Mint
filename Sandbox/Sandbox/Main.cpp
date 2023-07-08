#include "Main.h"


#include "Scenes/MainScene.h"
#include "BuiltinPlugins/FMODSoundSystem/FMODSoundSystemPlugin.h"



/// @brief Required function to setup scenes used in the game.
void register_scenes()
{
	using namespace mint;

	MINT_REGISTER_SCENE(CMainScene);	
}

/// @brief Required function to setup plugins used in the game.
void register_plugins()
{
	using namespace mint;

	MINT_REGISTER_PLUGIN(CFMODSoundSystemPlugin);
}

MINT_MAIN();