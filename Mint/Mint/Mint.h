#ifndef _MINT_MINT_ENGINE_H_
#define _MINT_MINT_ENGINE_H_


#include "IMint.h"


#include "Utility/ServiceSystem/ServiceLocator.h"
#include "Utility/Logging/Logging.h"
#include "Common/Timestep.h"
#include "SceneSystem/SceneManager.h"
#include "Utility/EventSystem/EventSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Utility/PluginSystem/PluginSystem.h"
#include "Graphics/Common/Color.h"
#include "Graphics/SceneRenderer/SceneRenderer.h"
#include "Graphics/Shaders/EmbeddedShaders/EmbeddedShaders.h"
#include "Graphics/Shaders/MaterialManager.h"
#include "UI/UI.h"
#include "Common/Random.h"
#include "Graphics/Common/Camera2D.h"
#include "Utility/Input/Input.h"
#include "Scripting/Common/ScriptLuaBindingServiceImpl.h"


#include "RessourceManagement/Common/RessourceLoaders/TextureLoader.h"
#include "RessourceManagement/Common/RessourceLoaders/ShaderLoader.h"
#include "RessourceManagement/Common/RessourceLoaders/ScriptLoader.h"
#include "RessourceManagement/Common/RessourceLoaders/BehaviorLoader.h"


#include "Scripting/BehaviorEngine.h"
#include "Scripting/ScriptEngine.h"

#if MINT_DISTR
#else
#include "Editor/Editor.h"
#endif


namespace mint
{


	class CMintEngine : public IMintEngine
	{
	public:
		MINT_DEFINE_ENGINE_CTOR(CMintEngine);


		bool initialize(const String& manifest_filepath) override final;

		void terminate() override final;

		void reset() override final;

		bool is_running() override final;

		void exit() override final;

		virtual bool is_in_editor_mode() { return false; }

		void begin_rendering() override final;

		void begin_frame() override final;

		void frame() override final;

		void ui_frame_begin() override final;

		void ui_frame_render() override final;

		void ui_frame_end() override final;

		void end_frame() override final;

		void end_rendering() override final;


		void on_before_update() override final;

		void on_update(f32 dt) override final;

		void on_after_update(f32 dt) override final;


		const CWindow& get_main_window_const() const override final;

		f32 get_engine_fps() override final;

		f32 get_engine_frametime() override final;

		void set_engine_fps(f32 fps) override final;

		void set_engine_window_title(const String& title) override final;

	private:
		bool m_running;

		CWindow m_mainWindow;

		CTimestep m_mainTimestep;



	private:
		bool _prepare_for_init();

		bool _load_app_manifiest(maml::CDocument& document, CWindow::SDescription& window_desc,
								 CPhysicsSystem::SDescription& physics_desc, CScene*& initial_scene);


		bool _init();

		bool _pre_init(CWindow::SDescription& wdesc, CPhysicsSystem::SDescription& pdesc);

		bool _post_init(IScene* current_scene);



		void _cleanup_after_terminate();

		void _pre_terminate();

		void _terminate();

		void _post_terminate();
	};


}


#define ENGINE() \
mint::IMintEngine::get_engine()


#endif