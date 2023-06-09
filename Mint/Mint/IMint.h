#ifndef _MINT_IMINT_H_
#define _MINT_IMINT_H_


#include "Components/CUCA.h"
#include "SceneSystem/SAS.h"
#include "Common/Window.h"


#define MINT_ENGINE_VERSION_MAJOR 2023
#define MINT_ENGINE_VERSION_MINOR 06
#define MINT_ENGINE_VERSION_PATCH 3
#define MINT_ENGINE_VERSION_STRING STRING(MINT_ENGINE_VERSION_MAJOR) "." STRING(MINT_ENGINE_VERSION_MINOR) "." STRING(MINT_ENGINE_VERSION_PATCH)


namespace mint
{
	
	class IMintEngine
	{
	public:
		static IMintEngine* get_engine();

		static void initialize_cuca_critical_sections();

		static void delete_cuca_critical_sections();

		static void register_component_importer_functions();

		static void register_component_exporter_functions();


		virtual bool initialize(const String&) = 0;

		virtual void terminate() = 0;

		virtual void reset() = 0;

		virtual bool is_running() = 0;

		virtual void exit() = 0;

		virtual bool is_in_editor_mode() = 0;

		virtual void print_engine_context() = 0;

		virtual void begin_rendering() = 0;

		virtual void end_rendering() = 0;


		virtual void frame() = 0;

		virtual void end_frame() = 0;


		virtual void ui_frame_begin() = 0;

		virtual void ui_frame_render() = 0;

		virtual void ui_frame_end() = 0;


		virtual void on_before_update() = 0;

		virtual void on_update(f32) = 0;

		virtual void on_after_update(f32) = 0;


		virtual const CWindow& get_main_window_const() const = 0;

		virtual CWindow& get_main_window_ref() = 0;

		virtual f32 get_engine_fps() = 0;

		virtual f32 get_engine_frametime() = 0;

		virtual void set_engine_fps(f32) = 0;

		virtual void set_engine_window_title(const String&) = 0;

		virtual void print_engine_rendering_pass_stack() = 0;

		virtual void print_engine_registered_importers_and_exporters() = 0;

	protected:
		static IMintEngine* s_engine;

	};

}


#define MINT_ENGINE() \
mint::IMintEngine::get_engine()


#define MINT_DEFINE_ENGINE_CTOR(class_name) \
class_name() : m_running(false) \
{ \
	IMintEngine::s_engine = this; \
} \


namespace mint
{
	// Collection of useful and not so useful functions for the engine.
}


#endif