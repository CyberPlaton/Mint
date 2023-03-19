#ifndef _MINT_PLUGIN_SYSTEM_H_
#define _MINT_PLUGIN_SYSTEM_H_


#include "Common/Common.h"

#include "Plugin.h"


namespace mint
{

	class CPluginSystem
	{
	public:
		STATIC_GET(CPluginSystem, g_CPluginSystem);

		bool initialize();

		void terminate();


		void register_plugin(IPlugin* plugin);


		void on_reset();

		void on_pre_initialization();

		void on_initialization();

		void on_post_initialization();

		void on_pre_termination();

		void on_termination();

		void on_post_termination();


		void on_before_update();

		void on_update();

		void on_after_update();

		void on_late_update();


	private:
		Vector< IPlugin* > m_registeredPlugins;

		Vector< IPlugin* > m_failedPlugins;

		Vector< IPlugin* > m_activePlugins;

		Vector< IPlugin* > m_preInitPlugins;
		Vector< IPlugin* > m_onInitPlugins;
		Vector< IPlugin* > m_postInitPlugins;



	private:
		void _pre_initialization();

		void _finalize_initialization();
	};


}


#define MINT_REGISTER_PLUGIN(class_name) \
mint::CPluginSystem::Get().register_plugin(new class_name())


#endif