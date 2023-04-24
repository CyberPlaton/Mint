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
		auto handle = mint::algorithm::djb_hash(program_name);

		// First check own storage.
		bool located_in_manager = lookup_shader_program(handle);
		if (located_in_manager)
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				const auto & shader = m_shaders.get_const(handle);

			);

			return shader;
		}

		// Secondly check in embedded shaders.
		bool located_in_embedded = fx::CEmbeddedShaders::Get().lookup_shader_program(handle);
		if (located_in_embedded)
		{
			return fx::CEmbeddedShaders::Get().get_embedded_shader_program(handle);
		}


		if (!located_in_manager && located_in_embedded)
		{
			MINT_ASSERT(false, "Could not locate shader program!");
		}
	}



	const Shader& CShaderManager::get_shader_program(ShaderHandle handle)
	{
		// First check own storage.
		bool located_in_manager = lookup_shader_program(handle);
		if (located_in_manager)
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				const auto & shader = m_shaders.get_const(handle);

			);

			return shader;
		}

		// Secondly check in embedded shaders.
		bool located_in_embedded = fx::CEmbeddedShaders::Get().lookup_shader_program(handle);
		if(located_in_embedded)
		{
			return fx::CEmbeddedShaders::Get().get_embedded_shader_program(handle);
		}


		if(!located_in_manager && located_in_embedded)
		{
			MINT_ASSERT(false, "Could not locate shader program!");
		}
	}


	bool CShaderManager::lookup_shader_program(const String& program_name)
	{
		return m_shaders.lookup(mint::algorithm::djb_hash(program_name));
	}


	bool CShaderManager::lookup_shader_program(ShaderHandle handle)
	{
		return m_shaders.lookup(handle);
	}


}