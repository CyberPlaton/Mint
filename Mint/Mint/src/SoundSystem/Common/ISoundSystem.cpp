#include "ISoundSystem.h"


namespace mint::sound
{

	mint::IPlugin::InitializationPoint CSoundSystem::get_initialization_point()
	{
		return InitializationPoint_Init;
	}

	mint::IPlugin::TerminationPoint CSoundSystem::get_termination_point()
	{
		return TerminationPoint_Shutdown;
	}

	CSoundSystem::CSoundSystem() : CPlugin("CSoundSystem"),
		m_currentEventCursor(0)
	{
	}

	void CSoundSystem::receive_incoming_event(SEvent* event)
	{
		m_currentEventCursor = (m_currentEventCursor + 1) % MINTSOUND_INCOMING_EVENT_COUNT_MAX;

		// Create a copy of the event for later processing.
		auto& copy = m_incomingEvents[m_currentEventCursor];

		copy.copy_from(event);
	}

	void CSoundSystem::on_before_update()
	{
	}

	void CSoundSystem::on_after_update(f32 dt)
	{
	}

	bool CSoundSystem::initialize()
	{
		m_incomingEvents.resize(MINTSOUND_INCOMING_EVENT_COUNT_MAX);

		return true;
	}

	void CSoundSystem::terminate()
	{
		m_incomingEvents.clear();
	}

}