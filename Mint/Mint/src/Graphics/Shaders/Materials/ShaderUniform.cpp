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


}