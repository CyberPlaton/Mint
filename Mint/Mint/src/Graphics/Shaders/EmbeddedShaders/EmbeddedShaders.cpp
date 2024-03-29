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
	}


	std::pair< const char*, const char* > CEmbeddedShaders::get_default_sprite_shader_program()
	{
		return m_shaders.get(mint::algorithm::djb_hash("Sprite"));
	}


	std::pair< const char*, const char* > CEmbeddedShaders::get_embedded_shader_program(const String& program_name)
	{
		auto h = mint::algorithm::djb_hash(program_name);

		return m_shaders.get(h);
	}


	std::pair< const char*, const char* > CEmbeddedShaders::get_embedded_shader_program(ShaderHandle handle)
	{
		return m_shaders.get(handle);
	}


	bool CEmbeddedShaders::_create_sprite_shader_program()
	{
		constexpr const char* vs =


			"#version 330                       \n"
			"in vec3 vertexPosition;            \n"
			"in vec2 vertexTexCoord;            \n"
			"in vec4 vertexColor;               \n"
			"out vec2 fragTexCoord;             \n"
			"out vec4 fragColor;                \n"
			"uniform mat4 mvp;                  \n"
			"void main()						\n"
			"{									\n"
			"    fragTexCoord = vertexTexCoord; \n"
			"    fragColor = vertexColor;       \n"
			"    gl_Position = mvp*vec4(vertexPosition, 1.0); \n"
			"}									\n";


		constexpr const char* fs =
			"#version 330						\n"
			"in vec2 fragTexCoord;              \n"
			"in vec4 fragColor;                 \n"
			"out vec4 finalColor;               \n"
			"uniform sampler2D texture0;        \n"
			"uniform vec4 colDiffuse;           \n"
			"void main()                        \n"
			"{                                  \n"
			"    vec4 texelColor = texture(texture0, fragTexCoord);   \n"
			"    finalColor = texelColor*colDiffuse*fragColor;        \n"
			"}                                  \n";


		auto h = mint::algorithm::djb_hash("Sprite");

		m_shaders.add(h, std::make_pair(vs, fs));

		return true;
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