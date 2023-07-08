#ifndef _MINT_PLUGIN_SYSTEM_H_
#define _MINT_PLUGIN_SYSTEM_H_


#include "Common/Common.h"
#include "Utility/Logging/Logging.h"
#include "Plugin.h"


namespace mint
{

	class CPluginSystem
	{
	public:
		STATIC_GET(CPluginSystem, g_CPluginSystem);

		bool initialize();

		void finalize_initialize();

		void terminate();


		void register_plugin(IPlugin* plugin);

		template< class T >
		T* get_plugin_as(const String& plugin_name);

		void print_registered_plugins();

		void print_active_and_failed_plugins();

		void on_reset();

		void on_pre_initialization();

		void on_initialization();

		void on_post_initialization();

		void on_pre_termination();

		void on_termination();

		void on_post_termination();


		void on_before_update();

		void on_update(f32 dt);

		void on_after_update(f32 dt);

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


	template< class T >
	T* mint::CPluginSystem::get_plugin_as(const String& plugin_name)
	{
		auto h = mint::algorithm::djb_hash(plugin_name);

		for (auto plugin : m_activePlugins)
		{
			if (plugin->get_plugin_identifier() == h) return reinterpret_cast<T*>(plugin);
		}

		return nullptr;
	}


}


#define MINT_REGISTER_PLUGIN(class_name) \
mint::CPluginSystem::Get().register_plugin(new class_name())


#endif