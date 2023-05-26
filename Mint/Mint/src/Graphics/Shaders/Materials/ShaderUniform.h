#ifndef _MINTFX_SHADER_UNIFORM_H_
#define _MINTFX_SHADER_UNIFORM_H_


#include "Common/Common.h"


namespace mint::fx
{
	
	struct SShaderUniform
	{
		template < typename T >
		void set(const String& uniform_name, const T& value, ShaderUniformDataType type)
		{
			m_name = uniform_name;
			m_type = type;
			m_data = &value;
		}

		template < typename T >
		T* get() const { return reinterpret_cast< T* >(m_data); }

		ShaderUniformDataType get_type() const;
		String get_name() const;
		const char* get_c_name() const;


		String m_name;
		ShaderUniformDataType m_type;
		void* m_data;
	};


}

#endif