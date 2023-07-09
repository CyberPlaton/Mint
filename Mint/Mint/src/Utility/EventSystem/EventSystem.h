#ifndef _MINT_EVENT_SYSTEM_H_
#define _MINT_EVENT_SYSTEM_H_


#include "Common/Common.h"
#include "Event.h"
#include "Delegate.h"
#include "Utility/STL/Map.h"
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

		void add_listener(SDelegate* delegate);

		void remove_listener(const String& listened_event_type, DelegateHandle delegate_identifier);

		void remove_listener(u64 listened_event_type, DelegateHandle delegate_identifier);

		void queue_event(SEvent* event);

	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		
		CMap< CMap< SDelegate* > > m_listeners;
		
		std::queue< SEvent* > m_eventQueue;

	};


}


#define MINT_DEFINE_EVENT_LISTENER(class_name, event_name, execute_function_impl) \
struct class_name : public mint::SDelegate \
{ \
	class_name() : mint::SDelegate(STRING(event_name), STRING(class_name)) \
	{ \
	} \
	void execute(mint::SEvent* event) override final \
	execute_function_impl \
}


#define MINT_DEFINE_PERSISTENT_EVENT_LISTENER(class_name, event_name, execute_function_impl) \
struct class_name : public mint::SDelegate \
{ \
	class_name() : mint::SDelegate(STRING(event_name), STRING(class_name), true) \
	{ \
	} \
	void execute(mint::SEvent* event) override final \
	execute_function_impl \
}


#define MINT_QUEUE_EVENT(event_name, ...) \
mint::CEventSystem::Get().queue_event(new SEvent(event_name, __VA_ARGS__))


#define MINT_REGISTER_EVENT_LISTENER(class_name) \
mint::CEventSystem::Get().add_listener(new class_name())


#endif