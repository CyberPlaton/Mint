#ifndef _MINT_SHADER_MANAGER_H_
#define _MINT_SHADER_MANAGER_H_


#include "Common/Common.h"
#include "Utility/STL/Map.h"
#include "Common/Algorithm.h"



namespace mint
{
	class CShaderManager
	{
	public:
		STATIC_GET(CShaderManager, s_CShaderManager);

		bool initialize();

		void terminate();

		void reset();


		bool add_shader_program(const String& program_name, Shader& shader);

		Shader get_shader_program(const String& program_name);
		Shader get_shader_program(ShaderHandle handle);


	private:
		static CShaderManager* s_CShaderManager;

		MINT_CRITICAL_SECTION(m_criticalSection);

		CMap< Shader > m_shaders;

	};
}


#endif