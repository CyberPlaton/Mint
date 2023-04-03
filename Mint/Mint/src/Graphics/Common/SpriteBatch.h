#ifndef _MINTFX_SPRITE_BATCH_H_
#define _MINTFX_SPRITE_BATCH_H_


#include "QuadBuffer.h"
#include "EmbeddedShaders/EmbeddedShaders.h"
#include "Common/Algorithm.h"
#include "Viewport.h"
#include "RessourceManagement/TextureManager.h"
#include "SceneSystem/Common/IScene.h"

namespace mint::fx
{

	class CSpriteBatch
	{
	public:
		CSpriteBatch();
		~CSpriteBatch();

		void start_batch(bgfx::ViewId view);

		void flush_batch();

		void next_batch() { flush_batch(); start_batch(m_view); }


		void draw_sprite(Vec2 position, f32 rotation, Vec2 scale, CColor color, Vec2 uv, TextureHandle texture);

		void draw_sprite(const Mat4& transform, CColor color, Vec2 uv, TextureHandle texture);


	private:
		CQuadBuffer m_qbuffer;

		TextureHandle m_previousTexture;

		ShaderProgramHandle m_spriteShader;

		bgfx::UniformHandle m_spriteTextureHandle;

		bgfx::ViewId m_view;

		Vec2 m_previousTextureSize;
	};

}


#endif