#ifndef _MINT_FRAME_ANIMATOR_H_
#define _MINT_FRAME_ANIMATOR_H_


#include "../Common/IAnimator.h"
#include "Graphics/Shaders/MaterialManager.h"


namespace mint::animation
{
	namespace frameanim
	{
		struct SFrameAnimationBehaviorData
		{
			SFrameAnimationBehaviorData& add_keyframe(MINT_ANIMATOR_FRAME_NUMBER x, MINT_ANIMATOR_FRAME_NUMBER y);

			SFrameAnimationBehaviorData& set_frame_count_x(MINT_ANIMATOR_FRAME_NUMBER x);

			SFrameAnimationBehaviorData& set_frame_count_y(MINT_ANIMATOR_FRAME_NUMBER y);


			Vector< std::pair< MINT_ANIMATOR_FRAME_NUMBER, MINT_ANIMATOR_FRAME_NUMBER > > m_keyframes;

			std::pair< MINT_ANIMATOR_FRAME_NUMBER, MINT_ANIMATOR_FRAME_NUMBER > m_currentFrame;

			MINT_ANIMATOR_FRAME_NUMBER m_currentFrameIndex = 0;

			u32 m_frameWidth = 0;
			u32 m_frameHeight = 0;

			MINT_ANIMATOR_FRAME_NUMBER m_frameCountX = 0;
			MINT_ANIMATOR_FRAME_NUMBER m_frameCountY = 0;
		};

		bool on_animator_initialize(CAnimator& animator, void* animation_data);
		void on_animator_terminate(CAnimator& animator, void* animation_data);
		bool on_animation_update(CAnimator& animator, f32 dt, void* animation_data);
		void on_animation_enter(CAnimator& animator, void* animation_data);
		void on_animation_exit(CAnimator& animator, void* animation_data);
	}

}

#endif