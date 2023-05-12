#ifndef _MINT_DELEGATE_H_
#define _MINT_DELEGATE_H_


#include "Event.h"


namespace mint
{
	struct IDelegate
	{
		virtual void execute(SEvent*) = 0;
	};


	struct SDelegate : public IDelegate
	{
		SDelegate(const String& listen_to_event_type, const String& delegate_name, bool persistent = false);

		virtual ~SDelegate() {}

		virtual void execute(SEvent*) override {};


		DelegateHandle get_unique_identifier() { return m_identifier; }

		u64 get_listening_event_type() { return m_eventType; }

		bool get_is_persistent() { return m_persistent; }

		u64 m_identifier;

		u64 m_eventType;

		bool m_persistent;
	};



}



#define MINT_REGISTER_EVENT_LISTENER(class_name) \
mint::CEventSystem::Get().add_listener(new class_name()) 



#define MINT_DEFINE_EVENT_LISTENER(class_name, event_name, execute_function) \
struct class_name : public mint::SDelegate \
{ \
	class_name() : SDelegate(STRING(event_name), STRING(class_name)) \
	{ \
	} \
\
	void execute(SEvent* event)	 \
	execute_function \
};


#endif