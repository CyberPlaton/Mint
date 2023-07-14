#ifndef _MINTSOUND_SOUND_SOURCE_SETTINGS_H_
#define _MINTSOUND_SOUND_SOURCE_SETTINGS_H_


#include "Common/Common.h"


namespace mint::sound
{
	enum SoundSourceChannelMode
	{
		SoundSourceChannelMode_Default = FMOD_DEFAULT,
		SoundSourceChannelMode_LoopOff = FMOD_LOOP_OFF,
		SoundSourceChannelMode_LoopNormal = FMOD_LOOP_NORMAL,
		SoundSourceChannelMode_LoopBidi = FMOD_LOOP_BIDI,
		SoundSourceChannelMode_3D_HeadRelative = FMOD_3D_HEADRELATIVE,
		SoundSourceChannelMode_3D_WorldRelative = FMOD_3D_WORLDRELATIVE,
		SoundSourceChannelMode_3D_InverseRolloff = FMOD_3D_INVERSEROLLOFF,
		SoundSourceChannelMode_3D_LinearRolloff = FMOD_3D_LINEARROLLOFF,
		SoundSourceChannelMode_3D_LinearSquareRolloff = FMOD_3D_LINEARSQUAREROLLOFF,
		SoundSourceChannelMode_3D_InverseTaperedRolloff = FMOD_3D_INVERSETAPEREDROLLOFF,
	};


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