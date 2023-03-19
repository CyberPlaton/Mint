#include "Event.h"

namespace mint
{
	SEvent::~SEvent()
	{
		m_eventArgs.clear();
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
}