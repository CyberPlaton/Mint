#ifndef _MINT_SHADER_MANAGER_H_
#define _MINT_SHADER_MANAGER_H_


#include "Graphics/Shaders/EmbeddedShaders/EmbeddedShaders.h"


namespace mint
{
	class CShaderManager
	{
	public:
		STATIC_GET(CShaderManager, s_CShaderManager);

		bool initialize();

		void terminate();

		void reset();


		void add_shader_program(const String& program_name, Shader& shader);

		const Shader& get_shader_program(const String& program_name);
		const Shader& get_shader_program(ShaderHandle handle);
		
		bool lookup_shader_program(const String& program_name);
		bool lookup_shader_program(ShaderHandle handle);


	private:
		static CShaderManager* s_CShaderManager;

		MINT_CRITICAL_SECTION(m_criticalSection);

		CMap< Shader > m_shaders;

	};
}


#endif