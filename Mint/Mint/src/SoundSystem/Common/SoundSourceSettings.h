#ifndef _MINTSOUND_SOUND_SOURCE_SETTINGS_H_
#define _MINTSOUND_SOUND_SOURCE_SETTINGS_H_


#include "Common/Common.h"


namespace mint::sound
{
	
	/// @brief Attributes and effects of a sound source, i.e. DSP effects such as 
	/// distortion, delay, echo etc.	
	struct SSoundSourceGroupSettings
	{
		f32 m_volume = 0.0f;
		f32 m_pitch = 0.0f;
		f32 m_pan = 0.0f;
		FMOD_MODE m_mode = FMOD_3D | FMOD_LOOP_OFF;
	};

}

#endif