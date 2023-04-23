#ifndef _MINT_MINT_ENGINE_H_
#define _MINT_MINT_ENGINE_H_


#include "IMint.h"


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