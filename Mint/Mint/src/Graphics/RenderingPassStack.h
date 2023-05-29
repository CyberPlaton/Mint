#ifndef _MINTFX_RENDERING_PASS_STACK_H_
#define _MINTFX_RENDERING_PASS_STACK_H_


#include "Common/IRenderingPass.h"
#include "Common/Algorithm.h"
#include "../IMint.h"


namespace mint::fx
{

	class CRenderingPassStack
	{
	public:
		CRenderingPassStack() = default;
		~CRenderingPassStack();

		bool initialize();

		void terminate();

		void reset();

		void on_resize(ICamera* render_camera);



		void on_begin_drawing();

		void on_frame(ICamera* render_camera, Vector< entt::entity >& entities);

		void on_frame_end();

		void combine_rendering_texture(RenderTexture2D& render_texture, IRenderingPass* rendering_pass);

		void begin_default_rendering_texture();

		void begin_default_blending_mode();

		void end_default_rendering_texture();

		void on_end_drawing();



		void on_ui_frame();

		void on_ui_frame_end();




		bool try_push_rendering_pass(IRenderingPass* pass);

		bool try_push_rendering_pass_after(const String& name, IRenderingPass* pass);

		void try_remove_rendering_pass(const String& name);



		void print_rendering_pass_stack();

	private:
		RenderTexture2D m_defaultRT;

		Vector< IRenderingPass* > m_renderingPasses;
	};

}

#endif