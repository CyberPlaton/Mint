#ifndef _MINT_GLOBAL_OS_SETTINGS_H_
#define _MINT_GLOBAL_OS_SETTINGS_H_


#include "Common.h"
#include "Common/Algorithm.h"
#include "Utility/Logging/Logging.h"
#include "Hardware/hwinfo/os.h"


namespace mint
{
	class CGlobalOSSettings
	{
	public:
		STATIC_GET(CGlobalOSSettings, s_CGlobalOSSettings);

		bool initialize();

		void print_os_context();

	private:
		hwinfo::OS m_os;
	};
}


#endif