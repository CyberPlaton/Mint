#ifndef _MINT_EVENT_SYSTEM_H_
#define _MINT_EVENT_SYSTEM_H_


#include "Common/Common.h"
#include "Event.h"
#include "Delegate.h"

#include "../ServiceSystem/Service.h"

#include <queue>


namespace mint
{
	class CMintEngine;

	class CEventSystem : public IService
	{
		friend class CMintEngine;

	public:
		STATIC_GET(CEventSystem, s_CEventSystem);

		bool initialize() override final;

		void terminate() override final;

		void reset() override final;

		String get_service_type() override final { return "Event System"; }

		void wait_for_termination();

		bool is_running();

		void set_should_update(bool value);

		void add_listener(const String& listened_event_type, SDelegate* delegate);

		void remove_listener(const String& listened_event_type, u64 delegate_identifier);

		void queue_event(SEvent* event);

	private:
		bool m_internalLoop;

		bool m_running;

		bool m_update;

		MINT_CRITICAL_SECTION(m_criticalSection);

		
		using EventDelegatesPair = std::pair< u64, Vector< SDelegate* > >;

		Vector< EventDelegatesPair > m_listeners;

		std::queue< SEvent* > m_eventQueue;


	private:
		void _run();

		void _internal_run();

		void _internal_computation();

		void _set_is_running(bool value);

		bool _should_update();
	};

}



#endif