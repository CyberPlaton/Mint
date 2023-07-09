#include "Delegate.h"

namespace mint
{
	SDelegate::SDelegate(const String& listen_to_event_type, const String& delegate_name, bool persistent/*= false*/) :
		m_identifier(mint::algorithm::djb_hash(delegate_name)),
		m_eventType(mint::algorithm::djb_hash(listen_to_event_type)),
		m_persistent(persistent)
	{
	}

	void SDelegate::execute(SEvent* event)
	{
		if (has_execute_function_set())
		{
			m_executeFunction(event);

			return;
		}

		MINT_ASSERT(false, "Invalid operation. Virtual delegate must either be overriden or having a execute function set!");
	}


	bool SDelegate::has_execute_function_set() const
	{
		return m_executeFunction != nullptr;
	}


	void SDelegate::set_execute_function(DelegateExecuteFunction function)
	{
		m_executeFunction = function;
	}

}