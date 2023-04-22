#ifndef _MINTFX_MATERIAL_H_
#define _MINTFX_MATERIAL_H_


#include "Common/Common.h"
#include "Utility/STL/Map.h"
#include "ShaderUniform.h"
#include "Common/Algorithm.h"


namespace mint::fx
{
	class CMaterial 
	{
	public:
		CMaterial();
		~CMaterial() = default;


		void bind_shader() const;
		void bind_blend_mode()  const;
		void bind_static_uniforms()  const;
		void bind_dynamic_uniforms()  const;


		void end_shader() const;
		void end_blend_mode() const;


		void set_blend_mode(BlendMode mode);
		void set_blend_mode_src_factor(uint32_t factor);
		void set_blend_mode_dst_factor(uint32_t factor);
		void set_blend_mode_equation(uint32_t equation);
		void restore_default_blend_mode();


		void set_fragment_shader(Shader shader);
		
		template < typename T >
		void add_static_uniform(const String& uniform_name, const T& value, ShaderUniformDataType type);

		template < typename T >
		void add_dynamic_uniform(const String& uniform_name, const T& value, ShaderUniformDataType type);


		Vec2 get_texture_dimension() const;

		TextureHandle get_texture_handle() const;


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