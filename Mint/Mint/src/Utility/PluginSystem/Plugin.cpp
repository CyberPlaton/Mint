#include "Plugin.h"


namespace mint
{

	CPlugin::CPlugin(const String& plugin_name)
	{
		m_identifier = mint::algorithm::djb_hash(plugin_name);
		m_name = plugin_name;
	}

	u64 CPlugin::get_plugin_identifier()
	{
		return m_identifier;
	}

	mint::String CPlugin::get_plugin_name()
	{
		return m_name;
	}

}

