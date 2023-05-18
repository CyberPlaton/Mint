#include "GlobalOSSettings.h"

namespace mint
{


	bool CGlobalOSSettings::initialize()
	{
		return true;
	}


	void CGlobalOSSettings::print_os_context()
	{
		MINT_LOG_INFO("Dumping operating system context information:");

		MINT_LOG_INFO("\tOS Name: {}", m_os.fullName());
		MINT_LOG_INFO("\tOS Version: {}, x64 := {}", m_os.version(),
													 m_os.is64bit() == true ? "True" : "False");

		MINT_LOG_INFO("\tOS Kernel: {}", m_os.kernel());

		MINT_LOG_SEPARATOR();
	}


}