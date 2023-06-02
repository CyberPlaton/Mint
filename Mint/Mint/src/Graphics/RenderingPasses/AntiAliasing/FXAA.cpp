#include "FXAA.h"


namespace mint::fx
{

	CFXAA::CFXAA() : CRenderingPass("CFXAA")
	{

	}

	bool CFXAA::initialize()
	{
		auto& window = MINT_ENGINE()->get_main_window_const();

		m_renderTexture = LoadRenderTexture(window.get_w(), window.get_h());


		bool result = CShaderManager::Get().load_shader_program("FXAA", &m_fxaaShader);

		if (result && IsRenderTextureReady(m_renderTexture))
		{
			auto blur = GetShaderLocation(m_fxaaShader, "blurThreshold");
			auto sat = GetShaderLocation(m_fxaaShader, "sat");

			SetShaderValue(m_fxaaShader, blur, (void*)&m_blurThreshold, SHADER_UNIFORM_FLOAT);
			SetShaderValue(m_fxaaShader, sat, (void*)&m_saturation, SHADER_UNIFORM_FLOAT);

			return true;
		}

 		return false;
	}

	void CFXAA::terminate()
	{
// 		UnloadRenderTexture(m_renderTexture);
// 		UnloadShader(m_fxaaShader);
	}

	void CFXAA::on_frame_end()
	{
		// Render current default RenderTexture to own RenderTexture with FXAA shader.
		auto& defaultRT = *__get_default_rendering_texture();

		BeginTextureMode(m_renderTexture);

		BeginShaderMode(m_fxaaShader);

		DrawTextureRec(defaultRT.texture, { 0.0f, 0.0f, (f32)defaultRT.texture.width, (f32)-defaultRT.texture.height }, { m_renderTextureOffset, m_renderTextureOffset }, WHITE);

		DrawTextureRec(defaultRT.texture, { 0.0f, 0.0f, (f32)defaultRT.texture.width, (f32)-defaultRT.texture.height }, { -m_renderTextureOffset, -m_renderTextureOffset }, WHITE);

		DrawTextureRec(defaultRT.texture, { 0.0f, 0.0f, (f32)defaultRT.texture.width, (f32)-defaultRT.texture.height }, { m_renderTextureOffset, -m_renderTextureOffset }, WHITE);

 		DrawTextureRec(defaultRT.texture, { 0.0f, 0.0f, (f32)defaultRT.texture.width, (f32)-defaultRT.texture.height }, { -m_renderTextureOffset, m_renderTextureOffset }, WHITE);

		EndShaderMode();

 		EndTextureMode();
	}

	void CFXAA::on_resize(ICamera* render_camera)
	{
		UnloadRenderTexture(m_renderTexture);

		m_renderTexture = LoadRenderTexture(render_camera->get_viewport_width(), render_camera->get_viewport_height());

		m_renderCamera = render_camera;
	}

	void CFXAA::on_frame_begin(ICamera* render_camera)
	{
		BeginTextureMode(m_renderTexture);

		ClearBackground(BLANK);

 		EndTextureMode();
	}

}