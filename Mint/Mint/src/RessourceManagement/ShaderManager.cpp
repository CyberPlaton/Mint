#include "ShaderManager.h"


namespace mint
{

	mint::CShaderManager* CShaderManager::s_CShaderManager = nullptr;


	bool CShaderManager::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		bool result = true;

		result &= fx::CEmbeddedShaders::Get().initialize();

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

			m_programs.reset();

		);
	}


	bool CShaderManager::add_shader_program(const String& program_name, ShaderProgramHandle& handle)
	{
		MINT_ASSERT(bgfx::isValid(handle) == true, "Passing an invalid handle!");

		auto h = mint::algorithm::djb_hash(program_name);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const bool result = m_programs.lookup(h);

		);


		if(result)
		{
			MINT_LOG_WARN("[{:.4f}][CShaderManager::add_shader_program] Attempt to add a duplicate Shader Program \"{}\"!", MINT_APP_TIME, program_name.c_str());
			return false;
		}


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_programs.add(h, handle);

		);

		return true;
	}


	mint::ShaderProgramHandle CShaderManager::get_shader_program(const String& program_name)
	{
		auto h = mint::algorithm::djb_hash(program_name);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const bool result = m_programs.lookup(h);

		);


		if(result)
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				const auto & handle = m_programs.get(h);

			);

			return handle;
		}


		MINT_LOG_WARN("[{:.4f}][CShaderManager::get_shader_program] Requested Shader Program \"{}\" was not found!", MINT_APP_TIME, program_name.c_str());
		return BGFX_INVALID_HANDLE;
	}



}