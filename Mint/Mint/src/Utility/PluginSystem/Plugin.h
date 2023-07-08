#ifndef _MINT_PLUGIN_H_
#define _MINT_PLUGIN_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"


namespace mint
{
	class IPlugin
	{
	public:
		enum InitializationPoint
		{
			InitializationPoint_PreInit = 0,
			InitializationPoint_Init,
			InitializationPoint_PostInit,
		};

		enum TerminationPoint
		{
			TerminationPoint_PreShutdown = 0,
			TerminationPoint_Shutdown,
			TerminationPoint_PostShutdown,
		};

	public:

		virtual InitializationPoint get_initialization_point() = 0;

		virtual TerminationPoint get_termination_point() = 0;

		virtual bool initialize() = 0;

		virtual void terminate() = 0;

		virtual void on_reset() = 0;

		virtual void on_before_update() = 0;

		virtual void on_update(f32) = 0;

		virtual void on_after_update(f32) = 0;

		virtual u64 get_plugin_identifier() = 0;

		virtual String get_plugin_name() = 0;
	};




	class CPlugin : public IPlugin
	{
	public:
		CPlugin(const String& plugin_name);


		virtual bool initialize() override { return false; }

		virtual void terminate() override {}

		virtual void on_reset() override {}

		virtual void on_before_update() override {}

		virtual void on_update(f32 dt) override {}

		virtual void on_after_update(f32 dt) override {}

		u64 get_plugin_identifier() override final;

		String get_plugin_name() override final;


	private:
		u64 m_identifier = 0;

		String m_name;
	};


}


#endif