#ifndef _MINTFX_GLOBAL_GRAPHICS_SETTINGS_H_
#define _MINTFX_GLOBAL_GRAPHICS_SETTINGS_H_


#include "Common/Common.h"
#include "Utility/STL/Map.h"
#include "Common/Algorithm.h"


namespace mint::fx::sc
{

	class CGlobalGraphicsSettings
	{
	public:
		STATIC_GET(CGlobalGraphicsSettings, s_CGlobalGraphicsSettings);

		bool initialize();

		
		void set_setting_value(const String& name, bool value);

		bool is_option_enabled(const String& name);





	public:
		CMap< bool > m_settings;
	};

}

#endif