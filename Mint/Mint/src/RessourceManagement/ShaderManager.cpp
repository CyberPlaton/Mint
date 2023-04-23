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


	void CShaderManager::add_shader_program(const String& program_name, Shader& shader)
	{
		auto h = mint::algorithm::djb_hash(program_name);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_shaders.add(h, shader);

		);
	}


	const Shader& CShaderManager::get_shader_program(const String& program_name)
	{
		auto h = mint::algorithm::djb_hash(program_name);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const auto& shader = m_shaders.get_const(h);

		);

		return shader;
	}



	const Shader& CShaderManager::get_shader_program(ShaderHandle handle)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const auto & shader = m_shaders.get_const(handle);

		);

		return shader;
	}


}