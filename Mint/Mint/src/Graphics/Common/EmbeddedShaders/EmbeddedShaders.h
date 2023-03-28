#ifndef _MINTFX_EMBEDDED_SHADERS_H_
#define _MINTFX_EMBEDDED_SHADERS_H_

#include "Common/Common.h"
#include "Utility/STL/Map.h"
#include "Common/Algorithm.h"
#include "Utility/Logging/Logging.h"

#include "bgfx/embedded_shader.h"


namespace mint::fx
{


	class CEmbeddedShaders
	{
	public:
		STATIC_GET(CEmbeddedShaders, s_CEmbeddedShaders);

		static bool create_embedded_shader(const mint::String& name, bgfx::RendererType::Enum renderer, 
										   const u8* vs_array_pointer, u32 vs_array_size, 
										   const u8* ps_array_pointer, u32 ps_array_size);

		static bool initialize();

		static void terminate();

		static mint::ShaderProgramHandle get_embedded_shader(const String& name);


		static mint::CMap< mint::ShaderProgramHandle > m_shaders;
	};


}


#endif