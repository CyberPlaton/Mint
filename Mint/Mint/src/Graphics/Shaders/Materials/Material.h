#ifndef _MINTFX_MATERIAL_H_
#define _MINTFX_MATERIAL_H_


#include "MaterialDefinition.h"
#include "RessourceManagement/ShaderManager.h"
#include "RessourceManagement/TextureManager.h"

#if MINT_DISTR
#else
namespace mint::editor { class CMaterialEditor; }
#endif


namespace mint::fx
{

	class CMaterial 
	{
#if MINT_DISTR
#else
		friend class mint::editor::CMaterialEditor;
#endif
	public:
		CMaterial(const SMaterialDefinition& definition);
		CMaterial();
		~CMaterial();

		void read_definition(const SMaterialDefinition& definition);

		void bind_shader() const;
		void bind_blend_mode()  const;
		void bind_static_uniforms()  const;
		void bind_dynamic_uniforms()  const;
		Vec2 get_texture_dimension() const;
		TextureHandle get_texture_handle() const;
		MaterialHandle get_material_handle() const;


		void end_shader() const;
		void end_blend_mode() const;


		void set_blend_mode(BlendMode mode);
		void set_blend_mode_src_factor(BlendingFactor factor);
		void set_blend_mode_dst_factor(BlendingFactor factor);
		void set_blend_mode_equation(BlendingEquation equation);
		void restore_default_blend_mode();

		void set_shader_program(const String& shader_program_name);

		void set_texture(const String& texture_name);
		void set_texture(TextureHandle texture_handle, const Vec2& texture_size);
		
		template < typename T >
		void add_static_uniform(const String& uniform_name, const T& value, ShaderUniformDataType type);

		template < typename T >
		void add_dynamic_uniform(const String& uniform_name, const T& value, ShaderUniformDataType type);


	private:
		MaterialHandle m_handle = SCAST(u64, -1);

		TextureHandle m_texture;
		
		Vec2 m_textureSize;
		
		mint::Shader m_shader;

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