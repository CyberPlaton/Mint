#include "ShaderUniform.h"

namespace mint::fx
{


	ShaderUniformDataType SShaderUniform::get_type() const
	{
		return m_type;
	}


	mint::String SShaderUniform::get_name() const
	{
		return m_name;
	}


	const char* SShaderUniform::get_c_name() const
	{
		return m_name.c_str();
	}

	void SShaderUniform::set(const String& uniform_name, void* value, ShaderUniformDataType type)
	{
		m_name = uniform_name;
		m_type = type;
		m_data = value;
	}

	void* SShaderUniform::get() const 
	{ 
		return m_data; 
	}
}