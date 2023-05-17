#ifndef _MINTFX_SHADER_GLOBAL_SETTINGS_H_
#define _MINTFX_SHADER_GLOBAL_SETTINGS_H_


#include "Common/Common.h"
#include "Utility/STL/Map.h"

namespace mint::fx::sc
{

	class CGlobalShaderSettings
	{
	public:
		STATIC_GET(CGlobalShaderSettings, s_CGlobalShaderSettings);

		bool initialize();


	public:
		bool m_opengl_330 = false;
		bool m_opengl_es20 = false;

		bool m_debug = false;
		bool m_release = false;
		bool m_distr = false;
	};

}

#endif