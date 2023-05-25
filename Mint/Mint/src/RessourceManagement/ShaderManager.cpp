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

	void CShaderManager::add_shader_program(const String& program_name, const String& vs_shader_path, const String& fs_shader_path)
	{
		auto h = mint::algorithm::djb_hash(program_name);

		m_shaders.add(h, std::make_pair(vs_shader_path, fs_shader_path));
	}


	bool CShaderManager::load_shader_program(const String& program_name, Shader* dest_shader)
	{
		auto h = mint::algorithm::djb_hash(program_name);

		return load_shader_program(h, dest_shader);
	}


	bool CShaderManager::load_shader_program(ShaderHandle handle, Shader* dest_shader)
	{
		if (fx::CEmbeddedShaders::Get().lookup_shader_program(handle))
		{
			// Load from embedded shaders.
			auto source_pair = fx::CEmbeddedShaders::Get().get_embedded_shader_program(handle);

			*dest_shader = LoadShaderFromMemory(source_pair.first, source_pair.second);

			MINT_ASSERT(dest_shader != nullptr, "Invalid operation. Could not load embedded shader!");

			if (dest_shader != nullptr)
			{
				return IsShaderReady(*dest_shader);
			}
		}
		else
		{
			// Load from file.
			if (m_shaders.lookup(handle))
			{
				auto pair = m_shaders.get(handle);

				if (!pair.first.empty())
				{
					*dest_shader = LoadShader(pair.first.c_str(), pair.second.c_str());
				}
				else
				{
					*dest_shader = LoadShader(nullptr, pair.second.c_str());
				}

				MINT_ASSERT(dest_shader != nullptr, "Invalid operation. Could not load shader from file!");

				if (dest_shader != nullptr)
				{
					return IsShaderReady(*dest_shader);
				}
			}
		}

		return false;
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