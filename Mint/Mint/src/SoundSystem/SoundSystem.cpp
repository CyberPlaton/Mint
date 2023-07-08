#include "SoundSystem.h"


namespace mint::sound
{
	void CSoundEngine::register_sound_system_implementation(CSoundSystem* system)
	{
		m_soundSystem = system;
	}
}

