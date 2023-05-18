#include "GlobalGraphicsSettings.h"

#include <gl/GL.h>

namespace mint
{

	bool CGlobalGraphicsSettings::initialize()
	{
		// Add all values to be added and set them to false.
		// Then cherrypick the ones that should be true.
		set_setting_value("opengl_330", false);
		set_setting_value("opengl_es20", false);
		set_setting_value("debug", false);
		set_setting_value("release", false);
		set_setting_value("distr", false);


#if MINTFX_OPENGL_330
		set_setting_value("opengl_330", true);
#elif MINTFX_OPENGL_ES20
		set_setting_value("opengl_es20", true);
#endif

#if MINT_DEBUG
		set_setting_value("debug", true);
#elif MINT_RELEASE
		set_setting_value("release", true);
#elif MINT_DISTR
		set_setting_value("distr", true);
#endif

		return true;
	}


	void CGlobalGraphicsSettings::set_setting_value(const String& name, bool value)
	{
		auto h = mint::algorithm::djb_hash(name);
		
		m_settings.add(h, value);
	}


	bool CGlobalGraphicsSettings::is_option_enabled(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);
		if(m_settings.lookup(h))
		{
			return m_settings.get(h);
		}

		return false;
	}


	void CGlobalGraphicsSettings::print_graphics_context()
	{
		MINT_LOG_INFO("Dumping graphics context information:");

		String vendor(reinterpret_cast< const char* > (glGetString(GL_VENDOR)));
		String renderer(reinterpret_cast<const char*> (glGetString(GL_RENDERER)));
		String opengl_version(reinterpret_cast<const char*> (glGetString(GL_VERSION)));
		String shader_version(reinterpret_cast<const char*> (glGetString(0x8B8C)));


		GLint totalMemoryKB;
		GLint freeMemoryKB;

		glGetIntegerv(0x9048, &totalMemoryKB);
		glGetIntegerv(0x9049, &freeMemoryKB);

		s32 total_memory_mb = totalMemoryKB / 1024;
		s32 free_memory_mb = freeMemoryKB / 1024;


		MINT_LOG_INFO("\tGPU Vendor: {}", vendor);
		MINT_LOG_INFO("\tGraphics Card: {}", renderer);
		MINT_LOG_INFO("\tGPU Memory: Total {} MB, Used: {} MB, Available: {} MB", total_memory_mb, (total_memory_mb - free_memory_mb), free_memory_mb);
		MINT_LOG_INFO("\tRenderer Information:");
		MINT_LOG_INFO("\t\tOpenGL Version: {}", opengl_version);
		MINT_LOG_INFO("\t\tOpenGL Shader: {}", shader_version);


		MINT_LOG_SEPARATOR();
	}

}