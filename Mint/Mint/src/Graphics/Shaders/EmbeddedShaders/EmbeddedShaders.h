#ifndef _MINTFX_EMBEDDED_SHADERS_H_
#define _MINTFX_EMBEDDED_SHADERS_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Utility/STL/Map.h"


namespace mint::fx
{

	class CEmbeddedShaders
	{
	public:
		STATIC_GET(CEmbeddedShaders, s_CEmbeddedShaders);

		bool initialize();

		void terminate();


		const Shader& get_default_sprite_shader_program();
		const Shader& get_embedded_shader_program(const String& program_name);
		const Shader& get_embedded_shader_program(ShaderHandle handle);

		bool lookup_shader_program(const String& program_name);
		bool lookup_shader_program(ShaderHandle handle);


	private:
		static CEmbeddedShaders* s_CEmbeddedShaders;

		CMap< Shader > m_shaders;


	private:

		bool _create_sprite_shader_program();
		bool _create_mask_shader_program();
		bool _create_wave_shader_program();

	};

}


#endif