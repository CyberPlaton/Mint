#include "ShaderManager.h"


namespace mint
{

	mint::CShaderManager* CShaderManager::s_CShaderManager = nullptr;


	bool CShaderManager::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		bool result = true;

		return result;
	}


	void CShaderManager::terminate()
	{
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CShaderManager::reset()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_shaders.reset();

		);
	}


	bool CShaderManager::add_shader_program(const String& program_name, Shader& handle)
	{
		return false;
	}


	Shader CShaderManager::get_shader_program(const String& program_name)
	{
		return Shader{ 0 };
	}



	Shader CShaderManager::get_shader_program(ShaderHandle handle)
	{
		return Shader{ 0 };
	}


}