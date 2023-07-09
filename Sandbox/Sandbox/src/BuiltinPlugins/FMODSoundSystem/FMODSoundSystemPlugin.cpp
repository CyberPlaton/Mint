#include "FMODSoundSystemPlugin.h"

CFMODSoundSystemPlugin::CFMODSoundSystemPlugin() : 
	m_system(nullptr)
{
}

CFMODSoundSystemPlugin::~CFMODSoundSystemPlugin()
{
}

bool CFMODSoundSystemPlugin::initialize()
{
	if (CSoundSystem::initialize() && FMOD::System_Create(&m_system) == FMOD_OK)
	{
		return true;
	}

	return false;
}

void CFMODSoundSystemPlugin::terminate()
{
	CSoundSystem::terminate();

	m_system->close();
	m_system->release();
}

void CFMODSoundSystemPlugin::on_reset()
{

}

void CFMODSoundSystemPlugin::on_update(mint::f32 dt)
{

}
