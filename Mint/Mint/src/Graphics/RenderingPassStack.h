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
		STATIC_GET(CRenderingPassStack,s_CRenderingPassStack);
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


		RenderTexture2D* get_default_render_texture();


		void on_ui_frame();

		void on_ui_frame_end();




		bool try_push_rendering_pass(IRenderingPass* pass);

		bool try_push_rendering_pass_after(const String& name, IRenderingPass* pass);

		void try_remove_rendering_pass(const String& name);

		template< class T >
		T* get_rendering_pass_as(const String& name);


		void print_rendering_pass_stack();

	private:
		RenderTexture2D m_defaultRT;

		Vector< IRenderingPass* > m_renderingPasses;
	};


	template< class T >
	T* mint::fx::CRenderingPassStack::get_rendering_pass_as(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);

		for (auto rp : m_renderingPasses)
		{
			if (rp->get_rendering_pass_id() == h) return reinterpret_cast< T* >(rp);
		}

		MINT_LOG_ERROR("[{:.4f}][CRenderingPassStack::get_rendering_pass_as] Requested Rendering Pass \"{}\" could not be located!", MINT_APP_TIME, name);

		return nullptr;
	}


}

#endif