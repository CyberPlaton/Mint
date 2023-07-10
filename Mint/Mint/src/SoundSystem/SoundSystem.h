#ifndef _MINT_SOUND_SYSTEM_H_
#define _MINT_SOUND_SYSTEM_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Utility/STL/Map.h"
#include "Utility/EventSystem/EventSystem.h"

#include "Common/SoundSourceGroup.h"
#include "Common/SoundSourceSettings.h"
#include "Common/SoundSource.h"


namespace mint::sound
{

	class CSoundEngine
	{
	public:
		STATIC_GET(CSoundEngine, s_CSoundEngine);

		bool initialize();

		void terminate();

		void reset();



		bool add_event_listener(const String& event_name);

		void remove_event_listener(const String& event_name);

		void propagate_received_event(SEvent* event);



	private:
		FMOD::System* m_system = nullptr;


		CMap< SDelegate* > m_delegates;

		Vector< SEvent > m_incomingEvents;
		
		u32 m_currentEventCursor;


	private:

	};



	namespace detail
	{
		void propagate_received_event(SEvent* event);
	}
}


#endif