#ifndef _MINT_SOUND_SYSTEM_H_
#define _MINT_SOUND_SYSTEM_H_


#include "Common/ISoundSystem.h"


namespace mint::sound
{

	class CSoundEngine
	{
	public:
		STATIC_GET(CSoundEngine, s_CSoundEngine);

		void register_sound_system_implementation(CSoundSystem* system);




	private:
		CSoundSystem* m_soundSystem = nullptr;

	};
}


#endif