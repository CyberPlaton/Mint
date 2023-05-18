#ifndef _MINT_CPU_H_
#define _MINT_CPU_H_


#include "Common.h"
#include "Algorithm.h"
#include "Utility/Logging/Logging.h"


#if MINT_PLATFORM_WINDOWS
#include <limits.h>
#include <intrin.h>
#else
#endif


namespace mint
{
	/// @brief Inspired by: https://gist.github.com/9prady9/a5e1e8bdbc9dc58b3349
	class CGlobalCPUSettings
	{
		struct CPUID
		{
			explicit CPUID(u32 function_identifier, u32 sub_function_identifier);

			const u32& _eax() const { return m_registers[0]; }
			const u32& _ebx() const { return m_registers[1]; }
			const u32& _ecx() const { return m_registers[2]; }
			const u32& _edx() const { return m_registers[3]; }


			u32 m_registers[4];
		};


	public:
		STATIC_GET(CGlobalCPUSettings, s_CGlobalCPUSettings);

		bool initialize();



		void print_cpu_context();

	private:
		String m_cpuVendor;
		String m_cpuModel;
		u32 m_cpuPhysicalCores = 0;
		u32 m_cpuLogicalCores = 0;
		u32 m_cpuClockSpeed = 0;

		u32 m_ramTotal = 0;
		u32 m_ramUsed = 0;

		bool m_hyperthreaded = false;

		bool m_intelCPU = false;
		bool m_amdCPU = false;

		u32 m_hfs = 0;

	private:
		static const u32 SSE_POS = 0x02000000;
		static const u32 SSE2_POS = 0x04000000;
		static const u32 SSE3_POS = 0x00000001;
		static const u32 SSE41_POS = 0x00080000;
		static const u32 SSE42_POS = 0x00100000;
		static const u32 AVX_POS = 0x10000000;
		static const u32 AVX2_POS = 0x00000020;
		static const u32 LVL_NUM = 0x000000FF;
		static const u32 LVL_TYPE = 0x0000FF00;
		static const u32 LVL_CORES = 0x0000FFFF;

	private:
		void _check_intel_logical_cores();

		void _check_amd_logical_cores();
	};
}

#endif