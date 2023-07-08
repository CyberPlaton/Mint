#include "ISoundSystem.h"


namespace mint::sound
{

	mint::IPlugin::InitializationPoint CSoundSystem::get_initialization_point()
	{
		return InitializationPoint_Init;
	}

	mint::IPlugin::TerminationPoint CSoundSystem::get_termination_point()
	{
		return TerminationPoint_Shutdown;
	}

	CSoundSystem::CSoundSystem() : CPlugin("CSoundSystem")
	{
	}

}