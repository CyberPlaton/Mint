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

		if (CShaderManager::Get().load_shader_program("Wave", &m_fxaaShader))
		{
			auto freqX = GetShaderLocation(m_fxaaShader, "freqX");
			auto freqY = GetShaderLocation(m_fxaaShader, "freqY");
			auto ampX = GetShaderLocation(m_fxaaShader, "ampX");
			auto ampY = GetShaderLocation(m_fxaaShader, "ampY");
			auto speedX = GetShaderLocation(m_fxaaShader, "speedX");
			auto speedY = GetShaderLocation(m_fxaaShader, "speedY");
			auto size = GetShaderLocation(m_fxaaShader, "size");
			auto secondes = GetShaderLocation(m_fxaaShader, "secondes");


			f32 vfreqX = 10.0f;
			f32 vfreqY = 10.0f;
			f32 vampX = 10.0f;
			f32 vampY = 10.0f;
			f32 vspeedX = 10.0f;
			f32 vspeedY = 10.0f;
			f32 vsecondes = 10.0f;
			Vec2 vsize = { window.get_w(), window.get_h() };

			SetShaderValue(m_fxaaShader, freqX, (void*)&vfreqX, SHADER_UNIFORM_FLOAT);
			SetShaderValue(m_fxaaShader, freqY, (void*)&vfreqY, SHADER_UNIFORM_FLOAT);
			SetShaderValue(m_fxaaShader, ampX, (void*)&vampX, SHADER_UNIFORM_FLOAT);
			SetShaderValue(m_fxaaShader, ampY, (void*)&vampY, SHADER_UNIFORM_FLOAT);
			SetShaderValue(m_fxaaShader, speedX, (void*)&vspeedX, SHADER_UNIFORM_FLOAT);
			SetShaderValue(m_fxaaShader, speedY, (void*)&vspeedY, SHADER_UNIFORM_FLOAT);
			SetShaderValue(m_fxaaShader, secondes, (void*)&vsecondes, SHADER_UNIFORM_FLOAT);
			SetShaderValue(m_fxaaShader, size, (void*)&vsize, SHADER_UNIFORM_VEC2);

			return true;
		}


// 		bool result = CShaderManager::Get().load_shader_program("FXAA", &m_fxaaShader);
// 
// 		if (result && IsRenderTextureReady(m_renderTexture))
// 		{
// 			// Set up uniforms for FXAA.
// 			auto loc = GetShaderLocation(m_fxaaShader, "frameResolution");
// 
// 			Vec2 resolution = { window.get_w(), window.get_h() };
// 
// 			SetShaderValue(m_fxaaShader, loc, (void*)&resolution, SHADER_UNIFORM_VEC2);
// 
// 			return true;
// 		}
// 
// 		return false;
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

		DrawTextureRec(defaultRT.texture, { 0.0f, 0.0f, (f32)defaultRT.texture.width, (f32)-defaultRT.texture.height }, { 0.0f, 0.0f }, WHITE);

		EndShaderMode();

		EndTextureMode();
	}

	void CFXAA::on_resize(ICamera* render_camera)
	{
		UnloadRenderTexture(m_renderTexture);

		m_renderTexture = LoadRenderTexture(render_camera->get_viewport_width(), render_camera->get_viewport_height());

		m_renderCamera = render_camera;

		// Set up uniforms for FXAA.
		auto loc = GetShaderLocation(m_fxaaShader, "frameResolution");

		Vec2 resolution = { render_camera->get_viewport_width(), render_camera->get_viewport_height() };

		SetShaderValue(m_fxaaShader, loc, (void*)&resolution, SHADER_UNIFORM_VEC2);
	}

}