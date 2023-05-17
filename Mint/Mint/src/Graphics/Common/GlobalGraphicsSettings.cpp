#include "GlobalGraphicsSettings.h"


namespace mint::fx::sc
{

	bool CGlobalGraphicsSettings::initialize()
	{
		// Add all values to be added and set them to false.
		// Then cherrypick the ones that should be true.
		set_setting_value("opengl_330", false);
		set_setting_value("opengl_es20", false);
		set_setting_value("debug", false);
		set_setting_value("release", false);
		set_setting_value("distr", false);


#if MINTFX_OPENGL_330
		set_setting_value("opengl_330", true);
#elif MINTFX_OPENGL_ES20
		set_setting_value("opengl_es20", true);
#endif

#if MINT_DEBUG
		set_setting_value("debug", true);
#elif MINT_RELEASE
		set_setting_value("release", true);
#elif MINT_DISTR
		set_setting_value("distr", true);
#endif

		return true;
	}


	void CGlobalGraphicsSettings::set_setting_value(const String& name, bool value)
	{
		auto h = mint::algorithm::djb_hash(name);
		
		m_settings.add(h, value);
	}


	bool CGlobalGraphicsSettings::is_option_enabled(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);
		if(m_settings.lookup(h))
		{
			return m_settings.get(h);
		}

		return false;
	}


}