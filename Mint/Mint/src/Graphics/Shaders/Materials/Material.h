#ifndef _MINTFX_MATERIAL_H_
#define _MINTFX_MATERIAL_H_


#include "Common/Common.h"
#include "Utility/STL/Map.h"
#include "ShaderUniform.h"
#include "Common/Algorithm.h"

#include "RessourceManagement/ShaderManager.h"
#include "RessourceManagement/TextureManager.h"


namespace mint::fx
{
	enum BlendingFactor
	{
		BlendingFactor_Zero						= 0,
		BlendingFactor_One						= 1, 
		BlendingFactor_SrcColor					= RL_SRC_COLOR,
		BlendingFactor_OneMinusSrcColor			= RL_ONE_MINUS_SRC_COLOR,
		BlendingFactor_SrcAlpha					= RL_SRC_ALPHA,
		BlendingFactor_OneMinusSrcAlpha			= RL_ONE_MINUS_SRC_ALPHA,
		BlendingFactor_DstAlpha					= RL_DST_ALPHA,
		BlendingFactor_DstColor					= RL_DST_COLOR,
		BlendingFactor_OneMinusDstAlpha			= RL_ONE_MINUS_DST_ALPHA,
		BlendingFactor_SrcAlphaSaturate			= RL_SRC_ALPHA_SATURATE,
		BlendingFactor_ConstantColor			= RL_CONSTANT_COLOR,
		BlendingFactor_OneMinusConstantColor	= RL_ONE_MINUS_CONSTANT_COLOR,
		BlendingFactor_ConstantAlpha			= RL_CONSTANT_ALPHA,
		BlendingFactor_OneMinusConstantAlpha	= RL_ONE_MINUS_CONSTANT_ALPHA,
		
		BlendingFactor_Count = BlendingFactor_OneMinusConstantAlpha
	};

	enum BlendingEquation
	{
		BlendingEquation_Add					= RL_FUNC_ADD,
		BlendingEquation_Min					= RL_MIN,
		BlendingEquation_Max					= RL_MAX,
		BlendingEquation_Subtract				= RL_FUNC_SUBTRACT,
		BlendingEquation_ReverseSubtract		= RL_FUNC_REVERSE_SUBTRACT,
		BlendingEquation_BlendEquationRGB		= RL_BLEND_EQUATION_RGB,
		BlendingEquation_BlendEquationAlpha		= RL_BLEND_EQUATION_ALPHA,
		BlendingEquation_BlendDstRGB			= RL_BLEND_DST_RGB,
		BlendingEquation_BlendSrcRGB			= RL_BLEND_SRC_RGB,
		BlendingEquation_BlendDstAlpha			= RL_BLEND_DST_ALPHA,
		BlendingEquation_BlendSrcAlpha			= RL_BLEND_SRC_ALPHA,
		BlendingEquation_BlendColor				= RL_BLEND_COLOR,

		BlendingEquation_Count = BlendingEquation_BlendColor
	};


	class CMaterial 
	{
	public:
		CMaterial();
		~CMaterial() = default;


		void bind_shader() const;
		void bind_blend_mode()  const;
		void bind_static_uniforms()  const;
		void bind_dynamic_uniforms()  const;
		Vec2 get_texture_dimension() const;
		TextureHandle get_texture_handle() const;


		void end_shader() const;
		void end_blend_mode() const;


		void set_blend_mode(BlendMode mode);
		void set_blend_mode_src_factor(BlendingFactor factor);
		void set_blend_mode_dst_factor(BlendingFactor factor);
		void set_blend_mode_equation(BlendingEquation equation);
		void restore_default_blend_mode();

		void set_shader_program(const String& shader_program_name);
		void set_shader_program(const Shader& shader);

		void set_texture(const String& texture_name);
		void set_texture(TextureHandle texture_handle, const Vec2& texture_size);
		
		template < typename T >
		void add_static_uniform(const String& uniform_name, const T& value, ShaderUniformDataType type);

		template < typename T >
		void add_dynamic_uniform(const String& uniform_name, const T& value, ShaderUniformDataType type);


	private:
		TextureHandle m_texture;
		
		Vec2 m_textureSize;
		
		Shader m_shader;

		BlendMode m_blendMode = BLEND_ALPHA;
		uint32_t m_srcBlendFactor = RL_SRC_ALPHA;
		uint32_t m_dstBlendFactor = RL_ONE_MINUS_SRC_ALPHA;
		uint32_t m_blendingEquation = RL_MAX;

		CMap< SShaderUniform > m_staticUniforms;
		CMap< SShaderUniform > m_dynamicUniforms;


	private:

		void _bind_uniforms(const CMap< SShaderUniform >& uniforms) const;
	};


	template < typename T >
	void mint::fx::CMaterial::add_dynamic_uniform(const String& uniform_name, const T& value, ShaderUniformDataType type)
	{
		auto h = mint::algorithm::djb_hash(uniform_name);


		SShaderUniform uniform;
		uniform.set(value, type);

		m_staticUniforms.add(h, uniform);
	}


	template < typename T >
	void mint::fx::CMaterial::add_static_uniform(const String& uniform_name, const T& value, ShaderUniformDataType type)
	{
		auto h = mint::algorithm::djb_hash(uniform_name);


		SShaderUniform uniform;
		uniform.set(value, type);

		m_dynamicUniforms.add(h, uniform);
	}


}


#endif