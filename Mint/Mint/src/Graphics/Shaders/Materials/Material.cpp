#include "Material.h"


namespace mint::fx
{


	CMaterial::CMaterial() : 
		m_texture(MINT_INVALID_HANDLE)
	{
	}


	void CMaterial::restore_default_blend_mode()
	{
		m_blendMode = BLEND_ALPHA;
		m_srcBlendFactor = RL_SRC_ALPHA;
		m_dstBlendFactor = RL_ONE_MINUS_SRC_ALPHA;
		m_blendingEquation = RL_MAX;
	}


	void CMaterial::set_blend_mode(BlendMode mode)
	{
		m_blendMode = mode;
	}


	void CMaterial::set_blend_mode_src_factor(uint32_t factor)
	{
		m_srcBlendFactor = factor;
	}


	void CMaterial::set_blend_mode_dst_factor(uint32_t factor)
	{
		m_dstBlendFactor = factor;
	}


	void CMaterial::bind_blend_mode()  const
	{
		rlSetBlendMode(m_blendMode);
		rlSetBlendFactors(m_srcBlendFactor, m_dstBlendFactor, m_blendingEquation);
		rlSetBlendMode(BLEND_CUSTOM);
	}


	void CMaterial::set_blend_mode_equation(uint32_t equation)
	{
		m_blendingEquation = equation;
	}


	void CMaterial::set_fragment_shader(Shader shader)
	{
		m_shader = shader;
	}

	void CMaterial::bind_static_uniforms()  const
	{
		_bind_uniforms(m_staticUniforms);
	}


	void CMaterial::bind_dynamic_uniforms()  const
	{
		_bind_uniforms(m_dynamicUniforms);
	}


	void CMaterial::_bind_uniforms(const CMap< SShaderUniform >& uniforms) const
	{
		uint32_t uniform_type = 0;
		uint32_t uniform_loc = 0;

		for (const auto& uniform: uniforms.get_all_const())
		{
			uniform_loc = GetShaderLocation(m_shader, uniform.get_c_name());

			switch (uniform.get_type())
			{
			case ShaderUniformDataType::SHADER_UNIFORM_FLOAT:
			{
				uniform_type = SHADER_UNIFORM_FLOAT;
				break;
			}
			case ShaderUniformDataType::SHADER_UNIFORM_INT:
			{
				uniform_type = SHADER_UNIFORM_INT;
				break;
			}
			case ShaderUniformDataType::SHADER_UNIFORM_IVEC2:
			case ShaderUniformDataType::SHADER_UNIFORM_VEC2:
			{
				uniform_type = SHADER_UNIFORM_VEC2;
				break;
			}
			case ShaderUniformDataType::SHADER_UNIFORM_IVEC3:
			case ShaderUniformDataType::SHADER_UNIFORM_VEC3:
			{
				uniform_type = SHADER_UNIFORM_VEC3;
				break;
			}
			case ShaderUniformDataType::SHADER_UNIFORM_IVEC4:
			case ShaderUniformDataType::SHADER_UNIFORM_VEC4:
			{
				uniform_type = SHADER_UNIFORM_VEC4;
				break;
			}
			case ShaderUniformDataType::SHADER_UNIFORM_SAMPLER2D:
			{
				uniform_type = SHADER_UNIFORM_SAMPLER2D;
				break;
			}
			default:
			{
				continue;
			}
			}

			SetShaderValue(m_shader, uniform_loc, uniform.m_data, uniform_type);
		}
	}


	void CMaterial::bind_shader()  const
	{
		BeginShaderMode(m_shader);
	}


	void CMaterial::end_shader() const
	{
		EndShaderMode();
	}


	void CMaterial::end_blend_mode() const
	{
		EndBlendMode();
	}


	mint::Vec2 CMaterial::get_texture_dimension() const
	{
		return m_textureSize;
	}


	mint::TextureHandle CMaterial::get_texture_handle() const
	{
		return m_texture;
	}


}