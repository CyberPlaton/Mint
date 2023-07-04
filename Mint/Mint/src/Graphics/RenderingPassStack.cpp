#include "RenderingPassStack.h"

namespace mint::fx
{

	CRenderingPassStack::~CRenderingPassStack()
	{
		terminate();
	}

	void CRenderingPassStack::on_frame_end()
	{
		EndTextureMode();
	}

	void CRenderingPassStack::try_remove_rendering_pass(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);

		// Find rendering pass in question.
		IRenderingPass* rendering_pass = nullptr;
		auto index = 0;
		for (auto& rp : m_renderingPasses)
		{
			if (rp->get_rendering_pass_id() == h)
			{
				// Erase from stack.
				rendering_pass = rp;

				m_renderingPasses.erase(m_renderingPasses.begin() + index);

				break;
			}
			index++;
		}

		// Delete if anything was found.
		if (rendering_pass)
		{
			delete rendering_pass; rendering_pass = nullptr;
		}
	}

	void CRenderingPassStack::print_rendering_pass_stack()
	{
		MINT_LOG_INFO("Dumping registered rendering passes from top to bottom:");

		for (auto& rp : m_renderingPasses)
		{
			MINT_LOG_INFO("\t\"{}\"", rp->get_rendering_pass_name());
		}

		MINT_LOG_SEPARATOR();
	}

	bool CRenderingPassStack::try_push_rendering_pass(IRenderingPass* pass)
	{
		if (pass->initialize())
		{
			m_renderingPasses.push_back(pass);

			return true;
		}

		pass->terminate();

		delete pass; pass = nullptr;

		return false;
	}

	bool CRenderingPassStack::try_push_rendering_pass_after(const String& name, IRenderingPass* pass)
	{
		auto h = mint::algorithm::djb_hash(name);

		// Find rendering pass in question.
		auto index = 0;
		for (auto& rp : m_renderingPasses)
		{
			if (rp->get_rendering_pass_id() == h)
			{
				// Insert new rendering pass after the found one.
				if (pass->initialize())
				{
					m_renderingPasses.insert(m_renderingPasses.begin() + index + 1, pass);

					return true;
				}
			}
			index++;
		}


		pass->terminate();

		delete pass; pass = nullptr;

		return false;
	}

	bool CRenderingPassStack::initialize()
	{
		auto& window = MINT_ENGINE()->get_main_window_const();

		m_defaultRT = LoadRenderTexture(window.get_w(), window.get_h());

		return IsRenderTextureReady(m_defaultRT);
	}

	void CRenderingPassStack::terminate()
	{
		while (!m_renderingPasses.empty())
		{
			auto rp = m_renderingPasses[0];

			m_renderingPasses.erase(m_renderingPasses.begin());

			if (rp->requires_termination())
			{
				rp->terminate();

				delete rp; rp = nullptr;
			}
		}

		UnloadRenderTexture(m_defaultRT);
	}

	void CRenderingPassStack::reset()
	{
		for (auto& rp : m_renderingPasses)
		{
			rp->reset();
		}
	}

	void CRenderingPassStack::on_resize(ICamera* render_camera)
	{
		UnloadRenderTexture(m_defaultRT);

		m_defaultRT = LoadRenderTexture(render_camera->get_viewport_width(), render_camera->get_viewport_height());

		for (auto& rp : m_renderingPasses)
		{
			rp->on_resize(render_camera);
		}
	}

	void CRenderingPassStack::on_frame(ICamera* render_camera, Vector< entt::entity >& entities)
	{
		begin_default_rendering_texture();

		ClearBackground(render_camera->get_clear_color().as_cliteral());

		render_camera->use_camera();

		for (auto& rp : m_renderingPasses)
		{
			rp->__set_default_rendering_texture(get_default_render_texture());

			rp->on_frame_begin(render_camera);

			rp->on_frame(entities);

			rp->on_frame_end();
			

			if (rp->uses_render_texture())
			{
				combine_rendering_texture(rp->get_render_texture(), rp);

				begin_default_rendering_texture();
			}
		}

		render_camera->end_camera();

		end_default_rendering_texture();
	}

	void CRenderingPassStack::combine_rendering_texture(RenderTexture2D& render_texture, IRenderingPass* rendering_pass)
	{
		// Render Render Texture on top of the default Render Texture.
		BeginTextureMode(m_defaultRT);


		rendering_pass->set_custom_combine_shader();

		rendering_pass->set_combine_blending_mode();


		DrawTextureRec(render_texture.texture, { 0.0f, 0.0f, (f32)render_texture.texture.width, (f32)-render_texture.texture.height }, { 0.0f, 0.0f }, WHITE);


		end_default_rendering_texture();


		if (rendering_pass->uses_custom_combine_shader()) rendering_pass->end_custom_combine_shader();
	}

	void CRenderingPassStack::begin_default_rendering_texture()
	{
		BeginTextureMode(m_defaultRT);

		begin_default_blending_mode();
	}

	void CRenderingPassStack::end_default_rendering_texture()
	{
		EndTextureMode();
	}

	void CRenderingPassStack::begin_default_blending_mode()
	{
		BeginBlendMode(BLEND_ALPHA);
	}

	void CRenderingPassStack::on_ui_frame()
	{
		BeginTextureMode(m_defaultRT);
	}

	void CRenderingPassStack::on_ui_frame_end()
	{
		EndTextureMode();
	}

	void CRenderingPassStack::on_begin_drawing()
	{
		begin_default_rendering_texture();
	}

	void CRenderingPassStack::on_end_drawing()
	{
		// Finish rendering passes and display to Screen.
		BeginDrawing();
 		
		DrawTextureRec(m_defaultRT.texture, { 0.0f, 0.0f, (f32)m_defaultRT.texture.width, (f32)-m_defaultRT.texture.height }, { 0.0f, 0.0f }, RAYWHITE);

		EndDrawing();
	}

	RenderTexture2D* CRenderingPassStack::get_default_render_texture()
	{
		return &this->m_defaultRT;
	}

}