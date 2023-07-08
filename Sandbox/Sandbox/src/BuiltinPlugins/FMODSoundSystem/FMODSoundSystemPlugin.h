#ifndef _FMOD_SOUND_SYSTEM_PLUGIN_H_
#define _FMOD_SOUND_SYSTEM_PLUGIN_H_


#include "SoundSystem/Common/ISoundSystem.h"
#include "FMOD/fmod.hpp"


class CFMODSoundSystemPlugin : public mint::sound::CSoundSystem
{
public:
	CFMODSoundSystemPlugin();
	~CFMODSoundSystemPlugin();


	bool initialize() override final;

	void terminate() override final;

	void on_reset() override final;

	void on_before_update() override final;

	void on_update(mint::f32 dt) override final;

	void on_after_update(mint::f32 dt) override final;

private:
	FMOD::System* m_system = nullptr;

};


#endif