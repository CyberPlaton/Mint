#include "PluginSystem.h"

namespace mint
{


	bool CPluginSystem::initialize()
	{
		_pre_initialization();

		return true;
	}


	void CPluginSystem::finalize_initialize()
	{
		_finalize_initialization();
	}


	void CPluginSystem::terminate()
	{
		while (!m_activePlugins.empty())
		{
			auto plugin = m_activePlugins.front();

			m_activePlugins.erase(m_activePlugins.begin());

			delete plugin; plugin = nullptr;
		}
	}


	void CPluginSystem::register_plugin(IPlugin* plugin)
	{
		m_registeredPlugins.push_back(plugin);
	}


	void CPluginSystem::print_registered_plugins()
	{
		MINT_LOG_INFO("Dumping registered plugins information:");

		MINT_LOG_INFO("\ton pre init:");
		for (auto plugin : m_preInitPlugins)
		{
			MINT_LOG_INFO("\t\tPlugin: {}", plugin->get_plugin_name());
		}
		MINT_LOG_INFO("\ton init:");
		for (auto plugin : m_onInitPlugins)
		{
			MINT_LOG_INFO("\t\tPlugin: {}", plugin->get_plugin_name());
		}
		MINT_LOG_INFO("\ton post init:");
		for (auto plugin : m_postInitPlugins)
		{
			MINT_LOG_INFO("\t\tPlugin: {}", plugin->get_plugin_name());
		}

		MINT_LOG_SEPARATOR();
	}


	void CPluginSystem::print_active_and_failed_plugins()
	{
		MINT_LOG_INFO("Dumping active plugins information:");

		for (auto plugin : m_activePlugins)
		{
			MINT_LOG_INFO("\tPlugin Name: {}", plugin->get_plugin_name());
		}


		MINT_LOG_WARN("Dumping failed plugins information:");

		for (auto plugin : m_failedPlugins)
		{
			MINT_LOG_INFO("\tPlugin Name: {}", plugin->get_plugin_name());
		}

		MINT_LOG_SEPARATOR();
	}

	void CPluginSystem::on_reset()
	{
		for(auto& plugin: m_activePlugins)
		{
			plugin->on_reset();
		}
	}


	void CPluginSystem::on_pre_initialization()
	{
		for(auto& plugin: m_preInitPlugins)
		{
			if(plugin->initialize())
			{
				m_activePlugins.push_back(plugin);
			}
			else
			{
				m_failedPlugins.push_back(plugin);
			}
		}

		m_preInitPlugins.clear();
	}


	void CPluginSystem::on_initialization()
	{
		for (auto& plugin : m_onInitPlugins)
		{
			if (plugin->initialize())
			{
				m_activePlugins.push_back(plugin);
			}
			else
			{
				m_failedPlugins.push_back(plugin);
			}
		}

		m_onInitPlugins.clear();
	}


	void CPluginSystem::on_post_initialization()
	{
		for (auto& plugin : m_postInitPlugins)
		{
			if (plugin->initialize())
			{
				m_activePlugins.push_back(plugin);
			}
			else
			{
				m_failedPlugins.push_back(plugin);
			}
		}

		m_postInitPlugins.clear();
	}


	void CPluginSystem::on_pre_termination()
	{
		for(auto& plugin : m_activePlugins)
		{
			if(plugin->get_termination_point() == IPlugin::TerminationPoint_PreShutdown)
			{
				plugin->terminate();
			}
		}
	}


	void CPluginSystem::on_termination()
	{
		for (auto& plugin : m_activePlugins)
		{
			if (plugin->get_termination_point() == IPlugin::TerminationPoint_Shutdown)
			{
				plugin->terminate();
			}
		}
	}


	void CPluginSystem::on_post_termination()
	{
		for (auto& plugin : m_activePlugins)
		{
			if (plugin->get_termination_point() == IPlugin::TerminationPoint_PostShutdown)
			{
				plugin->terminate();
			}
		}
	}


	void CPluginSystem::on_before_update()
	{
		for (auto& plugin : m_activePlugins)
		{
			plugin->on_before_update();
		}
	}


	void CPluginSystem::on_update(f32 dt)
	{
		for (auto& plugin : m_activePlugins)
		{
			plugin->on_update(dt);
		}
	}


	void CPluginSystem::on_after_update(f32 dt)
	{
		for (auto& plugin : m_activePlugins)
		{
			plugin->on_after_update(dt);
		}
	}


	void CPluginSystem::_pre_initialization()
	{
		for(auto& plugin: m_registeredPlugins)
		{
			switch(plugin->get_initialization_point())
			{
			case IPlugin::InitializationPoint_PreInit:
			{
				m_preInitPlugins.push_back(plugin);
				break;
			}
			case IPlugin::InitializationPoint_Init:
			{
				m_onInitPlugins.push_back(plugin);
				break;
			}
			case IPlugin::InitializationPoint_PostInit:
			{
				m_postInitPlugins.push_back(plugin);
				break;
			}
			default:
			{
				break;
			}
			}
		}

		m_registeredPlugins.clear();
	}


	void CPluginSystem::_finalize_initialization()
	{
		while(!m_failedPlugins.empty())
		{
			auto plugin = m_failedPlugins.front();

			m_failedPlugins.erase(m_failedPlugins.begin());

			delete plugin; plugin = nullptr;
		}
	}


}