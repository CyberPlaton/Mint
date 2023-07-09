#ifndef _MINT_SOUND_SYSTEM_H_
#define _MINT_SOUND_SYSTEM_H_


#include "Common/ISoundSystem.h"
#include "Utility/STL/Map.h"
#include "Utility/EventSystem/EventSystem.h"


namespace mint::sound
{

	class CSoundEngine
	{
	public:
		STATIC_GET(CSoundEngine, s_CSoundEngine);


		void register_sound_system_implementation(CSoundSystem* system);

		void reset();



		bool add_event_listener(const String& event_name);

		void remove_event_listener(const String& event_name);

		void propagate_received_event(SEvent* event);



	private:
		CSoundSystem* m_soundSystem = nullptr;

		CMap< SDelegate* > m_delegates;


	private:

	};



	namespace detail
	{
		void propagate_received_event(SEvent* event);
	}
}


#endif