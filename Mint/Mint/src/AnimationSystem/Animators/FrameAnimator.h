#ifndef _MINT_FRAME_ANIMATOR_H_
#define _MINT_FRAME_ANIMATOR_H_


#include "../Common/IAnimator.h"
#include "Graphics/Shaders/MaterialManager.h"


namespace mint::animation
{
	class CFrameAnimator : public CAnimator
	{
	public:
		CFrameAnimator(const String& animator_name, entt::entity entity);

		virtual bool on_initialize();

		virtual void on_terminate();

		virtual void on_animation_enter();

		virtual void on_animation_update(f32 dt);

		virtual void on_animation_exit();


		CFrameAnimator& add_keyframe(MINT_ANIMATOR_FRAME_NUMBER x, MINT_ANIMATOR_FRAME_NUMBER y);

		CFrameAnimator& set_frame_count_x(MINT_ANIMATOR_FRAME_NUMBER x);

		CFrameAnimator& set_frame_count_y(MINT_ANIMATOR_FRAME_NUMBER y);


	protected:
		Vector< std::pair< MINT_ANIMATOR_FRAME_NUMBER, MINT_ANIMATOR_FRAME_NUMBER > > m_keyframes;

		std::pair< MINT_ANIMATOR_FRAME_NUMBER, MINT_ANIMATOR_FRAME_NUMBER > m_currentFrame;

		MINT_ANIMATOR_FRAME_NUMBER m_currentFrameIndex = 0;

		f32 m_frameCounter = 0.0f;

		u32 m_frameWidth = 0;
		u32 m_frameHeight = 0;

		MINT_ANIMATOR_FRAME_NUMBER m_frameCountX = 0;
		MINT_ANIMATOR_FRAME_NUMBER m_frameCountY = 0;

	};

}

#endif