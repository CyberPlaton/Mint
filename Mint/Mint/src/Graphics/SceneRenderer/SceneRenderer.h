#ifndef _MINTFX_SCENE_RENDERER_H_
#define _MINTFX_SCENE_RENDERER_H_


#include "Common/Rectangle.h"

#include "../Common/RenderingPass.h"
#include "../Common/Color.h"
#include "../Common/QuadBuffer.h"
#include "../Common/EmbeddedShaders/EmbeddedShaders.h"

#include "../../SceneSystem/Common/IScene.h"
#include "../../Components/CUCA.h"
#include "../../RessourceManagement/TextureManager.h"


namespace mint::fx
{

	class CSceneRenderer
	{
	public:
		STATIC_GET(CSceneRenderer, s_CSceneRenderer);

		bool initialize();

		void terminate();


		void on_pre_render(ICamera* render_camera);

		void on_render(mint::IScene* scene);

		void on_render(Vector< entt::entity >& entities);

		void on_post_render();

		void on_resize(ICamera* render_camera);


	private:
		static CSceneRenderer* s_CSceneRenderer;

		ICamera* m_currentRenderCamera = nullptr;

		bgfx::ViewId m_defaultView = MINTFX_DEFAULT_VIEW;

		bgfx::ViewId m_uiView = 255;

		CQuadBuffer m_quadBuffer;

		TextureHandle m_currentTexture;

		bgfx::UniformHandle m_currentTextureUniform;

		Vec2 m_currentTextureSize;

		ShaderProgramHandle m_defaultShader;



		bgfx::ViewId m_backbufferView = MINTFX_FRAMEBUFFER_VIEW;

		bgfx::FrameBufferHandle m_backbuffer;

		TextureHandle m_backbufferTexture;

		bgfx::UniformHandle m_backbufferTextureUniform;

		ShaderProgramHandle m_backbufferShader;



		Vector< IRenderingPass* > m_renderingPasses;


	private:
		void _start_batch();

		void _flush_batch();

		void _next_batch();

		void _render_sprite(Mat4& transform, const CColor& color, const mint::CRect& rect, TextureHandle texture, bool flip_horizontal, bool flip_vertical);	
	
		void CSceneRenderer::_fullscreen_quad(f32 texture_width, f32 texture_height);
	};

}


#endif