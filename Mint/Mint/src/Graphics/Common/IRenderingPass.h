#ifndef _MINTFX_RENDERING_PASS_H_
#define _MINTFX_RENDERING_PASS_H_


#include "Common/Common.h"
#include "Common/Rectangle.h"
#include "../../SceneSystem/Common/IScene.h"
#include "../../Components/CUCA.h"
#include "../../RessourceManagement/TextureManager.h"
#include "../Shaders/MaterialManager.h"


namespace mint::fx
{
	class IRenderingPass
	{
	public:
		virtual bool initialize() = 0;

		virtual void terminate() = 0;

		virtual void reset() = 0;

		virtual void on_resize(ICamera*) = 0;


		virtual void on_frame_begin(ICamera*) = 0;
	
		virtual void on_frame(Vector< entt::entity >&) = 0;

		virtual void on_frame_end() = 0;


		virtual bool uses_render_texture() = 0;

		virtual RenderTexture2D get_render_texture() = 0;

		virtual void set_combine_blending_mode() = 0;

		virtual String get_rendering_pass_name() = 0;

		virtual u64 get_rendering_pass_id() = 0;
	};



	class CRenderingPass : public IRenderingPass
	{
	public:
		CRenderingPass(const String& rendering_pass_name);

		virtual bool initialize() { return false; }
		virtual void terminate() {}
		virtual void reset() {}
		virtual void on_resize(ICamera* render_camera);


		
		virtual void on_frame_begin(ICamera* render_camera);
		
		virtual void on_frame(Vector< entt::entity >& entities) {}
		
		virtual void on_frame_end(){}



		virtual bool uses_render_texture() { return false; }

		virtual RenderTexture2D get_render_texture() { return m_renderTexture; }

		virtual void set_combine_blending_mode() {}

		virtual String get_rendering_pass_name() { return m_renderingPassName; }

		virtual u64 get_rendering_pass_id() { return mint::algorithm::djb_hash(m_renderingPassName); }

	protected:
		String m_renderingPassName;

		ICamera* m_renderCamera;

		RenderTexture2D m_renderTexture;
	};


}

#endif