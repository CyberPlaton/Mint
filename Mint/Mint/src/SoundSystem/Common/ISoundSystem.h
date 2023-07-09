#ifndef _MINT_ISOUND_SYSTEM_H_
#define _MINT_ISOUND_SYSTEM_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Utility/PluginSystem/Plugin.h"
#include "Utility/EventSystem/Event.h"


namespace mint::sound
{
	class ISoundSystem
	{
	public:


	};


	class CSoundSystem : public ISoundSystem, public CPlugin
	{
	public:
		CSoundSystem();

		////////////////////////////////////////////////////////////////////
		// CSoundSystem interface.
		////////////////////////////////////////////////////////////////////




		////////////////////////////////////////////////////////////////////
		// CPlugin interface.
		////////////////////////////////////////////////////////////////////
		InitializationPoint get_initialization_point() override final;

		TerminationPoint get_termination_point() override final;

		virtual bool initialize() override;

		virtual void terminate() override;

		virtual void on_reset() override {}

		void on_before_update() override final;

		virtual void on_update(f32 dt) override {}

		void on_after_update(f32 dt) override final;


		////////////////////////////////////////////////////////////////////
		// ISoundSystem interface.
		////////////////////////////////////////////////////////////////////
		void receive_incoming_event(SEvent* event);





	protected:
		Vector< SEvent > m_incomingEvents;
		u32 m_currentEventCursor;

	};
}

#endif