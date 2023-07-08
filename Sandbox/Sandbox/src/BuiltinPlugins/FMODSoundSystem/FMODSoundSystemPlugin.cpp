#include "FMODSoundSystemPlugin.h"

CFMODSoundSystemPlugin::CFMODSoundSystemPlugin()
{

}

CFMODSoundSystemPlugin::~CFMODSoundSystemPlugin()
{

}

bool CFMODSoundSystemPlugin::initialize()
{
	if (FMOD::System_Create(&m_system) == FMOD_OK)
	{
		return true;
	}

	return false;
}

void CFMODSoundSystemPlugin::terminate()
{
	m_system->close();
	m_system->release();
}

void CFMODSoundSystemPlugin::on_reset()
{

}

void CFMODSoundSystemPlugin::on_before_update()
{

}

void CFMODSoundSystemPlugin::on_update(mint::f32 dt)
{

}

void CFMODSoundSystemPlugin::on_after_update(mint::f32 dt)
{

}
