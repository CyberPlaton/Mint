#include "ISoundSystem.h"


namespace mint::sound
{

	mint::IPlugin::InitializationPoint CSoundSystemInterface::get_initialization_point()
	{
		return InitializationPoint_Init;
	}

	mint::IPlugin::TerminationPoint CSoundSystemInterface::get_termination_point()
	{
		return TerminationPoint_Shutdown;
	}

	CSoundSystemInterface::CSoundSystemInterface() : CPlugin("CSoundSystemInterface"),
		m_currentEventCursor(0)
	{
	}

	void CSoundSystemInterface::receive_incoming_event(SEvent* event)
	{
		m_currentEventCursor = (m_currentEventCursor + 1) % MINTSOUND_INCOMING_EVENT_COUNT_MAX;

		// Create a copy of the event for later processing.
		auto& copy = m_incomingEvents[m_currentEventCursor];

		copy.copy_from(event);
	}

	void CSoundSystemInterface::on_before_update()
	{
	}

	void CSoundSystemInterface::on_after_update(f32 dt)
	{
	}

	bool CSoundSystemInterface::initialize()
	{
		m_incomingEvents.resize(MINTSOUND_INCOMING_EVENT_COUNT_MAX);

		return true;
	}

	void CSoundSystemInterface::terminate()
	{
		m_incomingEvents.clear();
	}

}