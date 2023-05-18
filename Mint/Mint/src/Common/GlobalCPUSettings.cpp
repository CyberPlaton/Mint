#include "GlobalCPUSettings.h"


namespace mint
{
	bool CGlobalCPUSettings::initialize()
	{
		return true;
	}


	void CGlobalCPUSettings::print_cpu_context()
	{
		MINT_LOG_INFO("Dumping cpu context information:");

		MINT_LOG_INFO("\tCPU Vendor: {}", m_cpu.getVendor());
		MINT_LOG_INFO("\tCPU Model: {}", m_cpu.getModelName());
		MINT_LOG_INFO("\tCPU Cores: Physical: {}, Logical: {}, Hyperthreading: {}", m_cpu.numPhysicalCores(), m_cpu.getNumLogicalCores(), m_cpu.instructionSet()._isHTT == true  ? "True" : "False");
		MINT_LOG_INFO("\tCPU Cachesize: {} MB", m_cpu.getCacheSize_Bytes() / (1024 * 1024));
		MINT_LOG_INFO("\tCPU Clockspeed: Regular: {} MHz, Maximal: {} MHz", m_cpu.getRegularClockSpeed_kHz() / 1000, m_cpu.getMaxClockSpeed_kHz() / 1000);

		MINT_LOG_INFO("\tRAM Vendor: {}", m_ram.getVendor());
		MINT_LOG_INFO("\tRAM Model: {}", m_ram.getModel());

		u64 ram_total = m_ram.getTotalSize_Bytes() / (1024 * 1024);
		u64 ram_free = m_ram.getAvailableMemory() / (1024 * 1024);
		u64 ram_used = ram_total - ram_free;

		MINT_LOG_INFO("\tRAM: Total: {} MB, Used: {} MB, Available: {} MB", ram_total, ram_used, ram_free);

		MINT_LOG_SEPARATOR();
	}

}