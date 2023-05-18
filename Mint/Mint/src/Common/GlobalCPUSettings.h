#ifndef _MINT_GLOBAL_CPU_SETTINGS_H_
#define _MINT_GLOBAL_CPU_SETTINGS_H_


#include "Common.h"
#include "Algorithm.h"
#include "Utility/Logging/Logging.h"
#include "Hardware/hwinfo/cpu.h"
#include "Hardware/hwinfo/ram.h"

namespace mint
{
	class CGlobalCPUSettings
	{
	public:
		STATIC_GET(CGlobalCPUSettings, s_CGlobalCPUSettings);

		bool initialize();


		void print_cpu_context();


	private:
		hwinfo::CPU m_cpu;
		hwinfo::RAM m_ram;
	};
}

#endif