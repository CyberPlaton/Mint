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


		void begin_frame() override final;

		void frame() override final;

		void end_frame() override final;



		void on_before_update() override final;

		void on_update(f32 dt) override final;

		void on_after_update(f32 dt) override final;


	private:
		bool m_running;


	private:
		bool _prepare_for_init();

		bool _pre_init(CWindow::SDescription& desc);

		bool _init();

		bool _post_init(IScene* current_scene);



		void _cleanup_after_terminate();

		void _pre_terminate();

		void _terminate();

		void _post_terminate();



		void _on_pre_update();

		void _on_update(f32 dt = CTimestep::get_fps());

		void _on_post_update();

		void _on_late_update();



		void _begin_frame();

		void _frame();

		void _end_frame();
	};


}


#endif