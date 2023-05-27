#ifndef _MINTFX_SHADER_UNIFORM_H_
#define _MINTFX_SHADER_UNIFORM_H_


#include "Common/Common.h"


namespace mint::fx
{
	
	struct SShaderUniform
	{
		void set(const String& uniform_name, void* value, ShaderUniformDataType type);

		void* get() const;

		template< typename T > 
		T get_as();

		ShaderUniformDataType get_type() const;
		
		String get_name() const;
		
		const char* get_c_name() const;


		String m_name;
		ShaderUniformDataType m_type;
		void* m_data;
	};


	template< typename T >
	T mint::fx::SShaderUniform::get_as()
	{
		if (m_data)
		{
			return reinterpret_cast<T>(m_data);
		}
	}


}

#endif