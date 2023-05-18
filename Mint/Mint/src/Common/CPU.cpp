#include "CPU.h"


namespace mint
{

	CGlobalCPUSettings::CPUID::CPUID(u32 function_identifier, u32 sub_function_identifier)
	{
#if MINT_PLATFORM_WINDOWS
		__cpuidex((s32*)m_registers, (s32)function_identifier, (s32)sub_function_identifier);
#else
		// Requires testing!
		asm volatile
			("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
				: "a" (funcId), "c" (subFuncId));
#endif
	}


	bool CGlobalCPUSettings::initialize()
	{
		// Vendor.
		CPUID id(0, 0);

		m_hfs = id._eax();

		m_cpuVendor += String((const char*)&id._ebx(), 4);
		m_cpuVendor += String((const char*)&id._edx(), 4);
		m_cpuVendor += String((const char*)&id._ecx(), 4);


		// SSE Instructions.
		CPUID id2(1, 0);
		m_hyperthreaded = id2._edx() & AVX_POS;


		// Logical cores.
		auto temp = m_cpuVendor;
		mint::algorithm::string_to_lower(temp);

		if(mint::algorithm::string_find_substr(temp, "intel") != std::string::npos)
		{
			m_intelCPU = true;
			_check_intel_logical_cores();
		}
		else if(mint::algorithm::string_find_substr(temp, "amd") != std::string::npos)
		{
			m_amdCPU = true;
			_check_amd_logical_cores();
		}
		else
		{
			return false;
		}

		// CPU Model.
		for(auto i = 0x80000002; i < 0x80000005; i++)
		{
			CPUID id(i, 0);
			m_cpuModel += String((const char*)&id._eax(), 4);
			m_cpuModel += String((const char*)&id._ebx(), 4);
			m_cpuModel += String((const char*)&id._ecx(), 4);
			m_cpuModel += String((const char*)&id._edx(), 4);
		}

		return true;
	}


	void CGlobalCPUSettings::print_cpu_context()
	{
		MINT_LOG_INFO("Dumping cpu context information:");

		MINT_LOG_INFO("\tCPU Vendor: {}", m_cpuVendor);
		MINT_LOG_INFO("\tCPU Model: {}", m_cpuVendor);
		MINT_LOG_INFO("\tCPU Cores: Physical: {}, Logical: {}, Hyperthreading: {}", m_cpuPhysicalCores, m_cpuLogicalCores, m_hyperthreaded == true  ? "True" : "False");
		MINT_LOG_INFO("\tCPU Clockspeed: {} MHz", m_cpuClockSpeed);
		
		MINT_LOG_SEPARATOR();
	}


	void CGlobalCPUSettings::_check_intel_logical_cores()
	{
		static constexpr u32 MAX_INTEL_TOP_LVL = 32;

		u32 smt_count = 0;

		if(m_hfs >= 11)
		{
			for(auto level = 0; level < MAX_INTEL_TOP_LVL; level++)
			{
				CPUID id(0x0B, level);

				u32 current_level = (LVL_TYPE & id._ecx()) >> 8;

				switch (current_level)
				{
				case 0x01:
				{
					smt_count = LVL_CORES & id._ebx(); break;
				}
				case 0x02:
				{
					m_cpuLogicalCores = LVL_CORES & id._ebx(); break;
				}
				}
			}

			m_cpuPhysicalCores = m_cpuLogicalCores / smt_count;
		}
		else
		{
			if(m_hfs >= 1)
			{
				CPUID id(1, 0);

				m_cpuLogicalCores = (id._ebx() >> 16) & 0xFF;

				if(m_hfs >= 4)
				{
					CPUID id(4, 0);
					m_cpuPhysicalCores = 1 + (id._eax() >> 26) & 0x3F;
				}
			}

			if(m_hyperthreaded)
			{
				if(!m_cpuPhysicalCores > 1)
				{
					m_cpuPhysicalCores = 1;
					m_cpuLogicalCores = (m_cpuLogicalCores >= 2) ? m_cpuLogicalCores : 2;
				}
			}
			else
			{
				m_cpuPhysicalCores = m_cpuLogicalCores = 1;
			}
		}
	}


	void CGlobalCPUSettings::_check_amd_logical_cores()
	{
		CPUID id(1, 0);

		if(m_hfs >= 1)
		{
			m_cpuLogicalCores = (id._ebx() >> 16) & 0xFF;

			if(CPUID(0x80000000, 0)._eax() >= 8)
			{
				m_cpuPhysicalCores = 1 + (CPUID(0x80000008, 0)._ecx() & 0xFF);
			}
		}
		if(m_hyperthreaded)
		{
			if(!m_cpuPhysicalCores > 1)
			{
				m_cpuPhysicalCores = 1;
				m_cpuLogicalCores = (m_cpuLogicalCores >= 2) ? m_cpuLogicalCores : 2;
			}
			else
			{
				m_cpuLogicalCores = m_cpuPhysicalCores = 1;
			}
		}
	}


}