#include "GlobalShaderSettings.h"


namespace mint::fx::sc
{

	bool CGlobalShaderSettings::initialize()
	{
#if MINTFX_OPENGL_330
		m_opengl_330 = true;
#elif MINTFX_OPENGL_ES_2
		m_opengl_es20 = true;
#endif


		return true;
	}


}