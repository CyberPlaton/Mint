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

#if MINT_DEBUG
		m_debug = true;
#elif MINT_RELEASE
		m_release = true;
#elif MINT_DISTR
		m_distr = true;
#endif

		return true;
	}


}