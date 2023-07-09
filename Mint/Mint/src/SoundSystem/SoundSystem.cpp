#include "SoundSystem.h"


namespace mint::sound
{
	void CSoundEngine::register_sound_system_implementation(CSoundSystem* system)
	{
		m_soundSystem = system;
	}

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
		m_soundSystem->receive_incoming_event(event);
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

	namespace detail
	{
		void propagate_received_event(SEvent* event)
		{
			CSoundEngine::Get().propagate_received_event(event);
		}
	}
	
}

