#ifndef _MINT_IMINT_H_
#define _MINT_IMINT_H_


#include "Utility/ServiceSystem/ServiceLocator.h"
#include "Utility/Logging/Logging.h"
#include "Common/Timestep.h"
#include "SceneSystem/SceneManager.h"
#include "Common/Window.h"
#include "Utility/EventSystem/EventSystem.h"
#include "Physics/PhysicsSystem.h"
#include "SceneSystem/SAS.h"
#include "Utility/PluginSystem/PluginSystem.h"
#include "Graphics/Common/Color.h"
#include "Graphics/SceneRenderer/SceneRenderer.h"
#include "UI/UI.h"
#include "Common/Random.h"
#include "Graphics/Common/Camera2D.h"
#include "Utility/Input/Input.h"

#include "RessourceManagement/Common/RessourceLoaders/TextureLoader.h"
#include "RessourceManagement/Common/RessourceLoaders/ShaderLoader.h"


namespace mint
{

	class IMintEngine
	{
	public:
		static IMintEngine* get_engine();

		static void initialize_cuca_critical_sections();

		static void delete_cuca_critical_sections();

		static void run_spatial_acceleration_structure();

		static void run_scripting_system();


		virtual bool initialize(const String&) = 0;

		virtual void terminate() = 0;

		virtual void reset() = 0;

		virtual bool is_running() = 0;

		virtual void exit() = 0;


		virtual void begin_rendering() = 0;

		virtual void end_rendering() = 0;


		virtual void begin_frame() = 0;

		virtual void frame() = 0;

		virtual void end_frame() = 0;


		virtual void ui_frame_begin() = 0;

		virtual void ui_frame_render() = 0;

		virtual void ui_frame_end() = 0;


		virtual void on_before_update() = 0;

		virtual void on_update(f32) = 0;

		virtual void on_after_update(f32) = 0;


		virtual const CWindow& get_main_window_const() const = 0;

		virtual f32 get_engine_fps() = 0;

		virtual f32 get_engine_frametime() = 0;

		virtual void set_engine_fps(f32) = 0;

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


#endif