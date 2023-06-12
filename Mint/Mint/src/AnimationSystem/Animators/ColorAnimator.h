#ifndef _MINT_COLOR_ANIMATOR_H_
#define _MINT_COLOR_ANIMATOR_H_


#include "../Common/IAnimator.h"


namespace mint::animation
{
	namespace coloranim
	{
		struct SColorAnimationBehaviorData
		{
			void set_base_color(const mint::fx::CColor& color) { m_baseColor = color; }
			void set_destination_color(const mint::fx::CColor& color) { m_destColor = color; }


			mint::fx::CColor m_baseColor;
			mint::fx::CColor m_destColor;
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