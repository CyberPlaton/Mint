#include "SoundSystem.h"


namespace mint::sound
{
	bool CSoundEngine::add_event_listener(const String& event_name)
	{
		auto h = mint::algorithm::djb_hash(event_name);

		if (!m_delegates.lookup(h))
		{
			auto delegate = new SDelegate(event_name, event_name + String("_Delegate"));

			delegate->set_execute_function(mint::sound::detail::propagate_received_event);

			m_delegates.add(h, delegate);

			CEventSystem::Get().add_listener(event_name, delegate);

			return true;
		}

		return false;
	}

	void CSoundEngine::propagate_received_event(SEvent* event)
	{
		// Receive incoming event.
		m_currentEventCursor = (m_currentEventCursor + 1) % MINTSOUND_INCOMING_EVENT_COUNT_MAX;

		// Create a copy of the event for later processing.
		auto& copy = m_incomingEvents[m_currentEventCursor];

		copy.copy_from(event);
	}

	void CSoundEngine::remove_event_listener(const String& event_name)
	{
		auto h = mint::algorithm::djb_hash(event_name);

		if (m_delegates.lookup(h))
		{
			m_delegates.remove(h);

			auto id = mint::algorithm::djb_hash(event_name + String("_Delegate"));

			CEventSystem::Get().remove_listener(event_name, id);
		}
	}

	void CSoundEngine::reset()
	{
		// Unregister delegates from event system.
		// Note: This may be redundant, as the event system is doing the same on reset.
		for (auto delegate : m_delegates.get_all_const())
		{
			CEventSystem::Get().remove_listener(delegate->get_listening_event_type(), delegate->get_unique_identifier());
		}

		// We are responsible for freeing used memory from delegates.
		auto& delegates = m_delegates.get_all();
		while (!delegates.empty())
		{
			auto d = delegates[0];

			delete d; d = nullptr;

			delegates.erase(delegates.begin());
		}
	}

	bool CSoundEngine::initialize()
	{
		if (FMOD::System_Create(&m_system) == FMOD_OK)
		{
			m_incomingEvents.resize(MINTSOUND_INCOMING_EVENT_COUNT_MAX);
			return true;
		}

		return false;
	}

	void CSoundEngine::terminate()
	{
		m_incomingEvents.clear();
		m_system->close();
		m_system->release();
	}

	namespace detail
	{
		void propagate_received_event(SEvent* event)
		{
			CSoundEngine::Get().propagate_received_event(event);
		}
	}
	
}

