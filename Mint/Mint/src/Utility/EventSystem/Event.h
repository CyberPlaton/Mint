#ifndef _MINT_EVENT_H_
#define _MINT_EVENT_H_


#include "Common/Algorithm.h"
#include "Common/Any.h"
#include "Common/Timer.h"


namespace mint
{
	struct SEvent
	{
		template< typename... ARGS >
		SEvent(const String& event_type, ARGS&&... args);

		SEvent(const String& event_type);

		SEvent(const String& event_type, Vector< CAny >& data);

		SEvent(const SEvent& other) = delete;
		SEvent& operator=(const SEvent& other) = delete;

		~SEvent();

		u64 get_time_stamp() { return m_eventTimestamp; }

		u64 get_event_type() { return m_eventType; }

		CAny& get_event_data(u32 index) { MINT_ASSERT(index < m_eventArgs.size(), "Index out-of-bounds!"); return m_eventArgs[index]; }


		u64 m_eventType;

		Vector< CAny > m_eventArgs;

		u64 m_eventTimestamp;
	};



	template< typename... ARGS >
	mint::SEvent::SEvent(const String& event_type, ARGS&&... args)
	{
		m_eventTimestamp = CTimer::get_time();

		m_eventType = mint::algorithm::djb_hash(event_type);

		const CAny args_array[] = { args... };
		for(auto& arg: args_array)
		{
			m_eventArgs.push_back(std::move(arg));
		}
	}


}


#endif