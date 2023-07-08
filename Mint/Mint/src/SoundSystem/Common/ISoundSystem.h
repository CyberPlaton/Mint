#ifndef _MINT_ISOUND_SYSTEM_H_
#define _MINT_ISOUND_SYSTEM_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Utility/PluginSystem/Plugin.h"


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

		InitializationPoint get_initialization_point() override final;

		TerminationPoint get_termination_point() override final;


		virtual bool initialize() override { return false; }

		virtual void terminate() override {}

		virtual void on_reset() override {}

		virtual void on_before_update() override {}

		virtual void on_update(f32 dt) override {}

		virtual void on_after_update(f32 dt) override {}




	};
}

#endif