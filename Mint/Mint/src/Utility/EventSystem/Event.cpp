#include "Event.h"

namespace mint
{
	SEvent::~SEvent()
	{
		m_eventArgs.clear();
	}


	void SEvent::copy_from(SEvent* event)
	{
		m_eventType = event->get_event_type();
		m_eventArgs = event->m_eventArgs;
		m_eventTimestamp = event->get_time_stamp();
	}

	SEvent::SEvent(const String& event_type) :
		m_eventType(mint::algorithm::djb_hash(event_type))
	{
		m_eventTimestamp = CTimer::get_time();
	}


	SEvent::SEvent(const String& event_type, Vector< CAny >& data) :
		m_eventArgs(data), m_eventType(mint::algorithm::djb_hash(event_type))
	{
		m_eventTimestamp = CTimer::get_time();
	}

	SEvent::SEvent(u64 event_type) : 
		m_eventType(event_type)
	{
		m_eventTimestamp = CTimer::get_time();
	}

}