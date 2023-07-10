#ifndef _MINTSOUND_SOUND_SOURCE_SETTINGS_H_
#define _MINTSOUND_SOUND_SOURCE_SETTINGS_H_


#include "SoundCommon.h"


namespace mint::sound
{
	/// @brief Attributes and effects of a sound source, i.e. DSP effects such as 
	/// distortion, delay, echo etc., Reverb, Volume and Pitch and so on.
	/// Pure data interface. Each Sound Engine implementation overrides this to accomodate requirements.
	struct SSoundSourceSettings
	{
	};


}

#endif