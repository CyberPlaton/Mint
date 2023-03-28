#ifndef _MINT_EVENT_SYSTEM_H_
#define _MINT_EVENT_SYSTEM_H_


#include "Common/Common.h"
#include "Event.h"
#include "Delegate.h"

#include "UI/Common/UICommon.h"

#include <queue>


namespace mint
{
	class IMintEngine;

	class CEventSystem
	{
		friend class IMintEngine;

	public:
		STATIC_GET(CEventSystem, s_CEventSystem);

		bool initialize();

		void terminate();

		void reset();

		void update();

		void add_listener(const String& listened_event_type, SDelegate* delegate);

		void remove_listener(const String& listened_event_type, u64 delegate_identifier);

		void queue_event(SEvent* event);

	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		
		using EventDelegatesPair = std::pair< u64, Vector< SDelegate* > >;

		Vector< EventDelegatesPair > m_listeners;

		std::queue< SEvent* > m_eventQueue;

	};

}



#endif