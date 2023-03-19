#include "Delegate.h"

namespace mint
{
	SDelegate::SDelegate(const String& listen_to_event_type, const String& delegate_name) :
		m_identifier(mint::algorithm::djb_hash(delegate_name)),
		m_eventType(mint::algorithm::djb_hash(listen_to_event_type))
	{
	}
}