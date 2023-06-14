#ifndef _MINT_SCALE_ANIMATOR_H_
#define _MINT_SCALE_ANIMATOR_H_


#include "../Common/IAnimator.h"


namespace mint::animation
{
	namespace scaleanim
	{
		struct SScaleAnimationBehaviorData
		{
			void set_base_scale(Vec2 value) { m_baseScale = value; }
			void set_destination_scale(Vec2 value) { m_destScale = value; }


			Vec2 m_baseScale;
			Vec2 m_destScale;
			bool m_forward = true;
		};

		bool on_animator_initialize(CAnimator& animator, void* animation_data);
		void on_animator_terminate(CAnimator& animator, void* animation_data);
		bool on_animation_update(CAnimator& animator, f32 dt, void* animation_data);
		void on_animation_enter(CAnimator& animator, void* animation_data);
		void on_animation_exit(CAnimator& animator, void* animation_data);
	}

}

#endif