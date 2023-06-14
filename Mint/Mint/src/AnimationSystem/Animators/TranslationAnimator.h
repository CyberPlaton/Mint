#ifndef _MINT_TRANSLATION_ANIMATOR_H_
#define _MINT_TRANSLATION_ANIMATOR_H_


#include "../Common/IAnimator.h"


namespace mint::animation
{
	namespace translationanim
	{
		struct STranslationAnimationBehaviorData
		{
			void set_base_translation(Vec2 value) { m_baseTranslation = value; }
			void set_destination_translation(Vec2 value) { m_destTranslation = value; }


			Vec2 m_baseTranslation;
			Vec2 m_destTranslation;
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