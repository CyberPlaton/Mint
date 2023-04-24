#include "EmbeddedShaders.h"


namespace mint::fx
{

	mint::fx::CEmbeddedShaders* CEmbeddedShaders::s_CEmbeddedShaders = nullptr;


	bool CEmbeddedShaders::initialize()
	{
		bool result = true;

		result &= _create_sprite_shader_program();

#if MINTFX_USE_EMBEDDED_SHADERS
		result &= _create_mask_shader_program();
		result &= _create_wave_shader_program();
#endif

		return result;
	}


	void CEmbeddedShaders::terminate()
	{
		auto& shaders = m_shaders.get_all();

		while(!shaders.empty())
		{
			UnloadShader(shaders[0]);

			shaders.erase(shaders.begin());
		}
	}


	const Shader& CEmbeddedShaders::get_default_sprite_shader_program()
	{
		return m_shaders.get(mint::algorithm::djb_hash("Sprite"));
	}


	const Shader& CEmbeddedShaders::get_embedded_shader_program(const String& program_name)
	{
		auto h = mint::algorithm::djb_hash(program_name);

		return m_shaders.get(h);
	}


	const Shader& CEmbeddedShaders::get_embedded_shader_program(ShaderHandle handle)
	{
		return m_shaders.get(handle);
	}


	bool CEmbeddedShaders::_create_sprite_shader_program()
	{
		constexpr const char* vs =
			"#version 330"
			"in vec3 vertexPosition;"
			"in vec2 vertexTexCoord;"
			"in vec4 vertexColor;"
			"uniform mat4 mvp;"
			"out vec2 fragTexCoord;"
			"out vec4 fragColor;"
			"void main()"
			"{"
			"	fragTexCoord = vertexTexCoord;"
			"	fragColor = vertexColor;"
			"	gl_Position = mvp * vec4(vertexPosition, 1.0);"
			"}";


		constexpr const char* fs =
			"#version 330"
			"in vec2 fragTexColor;"
			"in vec4 fragColor;"
			"uniform sampler2D texture0;"
			"uniform vec4 colDiffuse;"
			"out vec4 finalColor;"
			"void main()"
			"{"
			"	vec4 texelColor = texture(texture0, fragTexCoord);"
			"	finalColor = texelColor * colDiffuse;"
			"}";

		Shader shader = LoadShaderFromMemory(vs, fs);

		if(IsShaderReady(shader))
		{
			auto h = mint::algorithm::djb_hash("Sprite");

			m_shaders.add(h, shader);

			return true;
		}

		MINT_LOG_ERROR("[{:.4f}][CEmbeddedShaders::_create_sprite_shader_program] Failed creating embedded shader program \"Sprite\"!", MINT_APP_TIME);

		return false;
	}


	bool CEmbeddedShaders::_create_mask_shader_program()
	{
		return true;
	}


	bool CEmbeddedShaders::_create_wave_shader_program()
	{
		return true;
	}


	bool CEmbeddedShaders::lookup_shader_program(const String& program_name)
	{
		return m_shaders.lookup(mint::algorithm::djb_hash(program_name));
	}


	bool CEmbeddedShaders::lookup_shader_program(ShaderHandle handle)
	{
		return m_shaders.lookup(handle);
	}


}