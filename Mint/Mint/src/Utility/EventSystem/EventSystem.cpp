#include "EventSystem.h"


namespace mint
{


	bool CEventSystem::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}


	void CEventSystem::terminate()
	{
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CEventSystem::reset()
	{
		// Completely clear the event queue.
		while (!m_eventQueue.empty())
		{
			auto event = m_eventQueue.front();

			m_eventQueue.pop();

			delete event;
			event = nullptr;
		}

		// Selectively clear listeners that are not persistent.
		for (auto& map : m_listeners.get_all())
		{
			Vector< u64 > to_be_removed;

			for (auto& delegate : map.get_all())
			{
				if (!delegate->get_is_persistent()) mint::algorithm::vector_push_back(to_be_removed, delegate->get_unique_identifier());
			}


			for (auto id : to_be_removed)
			{
				map.remove(id);
			}
		}
	}


	void CEventSystem::update()
	{
		while (!m_eventQueue.empty())
		{
			auto event = m_eventQueue.front();
			auto type = event->get_event_type();

			if(m_listeners.lookup(type))
			{
				auto& listeners = m_listeners.get_ref(type);

				for (auto& delegate : listeners.get_all())
				{
					delegate->execute(event);
				}
			}


			m_eventQueue.pop();
			delete event; event = nullptr;
		}
	}


	void CEventSystem::add_listener(const String& listened_event_type, SDelegate* delegate)
	{
		auto h = mint::algorithm::djb_hash(listened_event_type);
		auto id = delegate->get_unique_identifier();

		if (!m_listeners.lookup(h))
		{
			m_listeners.add(h, CMap< SDelegate* >{});
		}


		auto& map = m_listeners.get_ref(h);

		map.add(id, delegate);
	}


	void CEventSystem::add_listener(SDelegate* delegate)
	{
		auto h = delegate->get_listening_event_type();
		auto id = delegate->get_unique_identifier();

		if(!m_listeners.lookup(h))
		{
			m_listeners.add(h, CMap< SDelegate* >{});
		}


		auto& map = m_listeners.get_ref(h);

		map.add(id, delegate);
	}


	void CEventSystem::remove_listener(const String& listened_event_type, DelegateHandle delegate_identifier)
	{
		auto h = mint::algorithm::djb_hash(listened_event_type);
		
		remove_listener(h, delegate_identifier);
	}


	void CEventSystem::remove_listener(u64 listened_event_type, DelegateHandle delegate_identifier)
	{
		if (!m_listeners.lookup(listened_event_type)) return;

		auto& map = m_listeners.get_ref(listened_event_type);

		map.remove(delegate_identifier);
	}

	void CEventSystem::queue_event(SEvent* event)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_eventQueue.push(event);

		);
	}

}