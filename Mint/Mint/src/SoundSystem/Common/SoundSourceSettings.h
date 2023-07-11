#ifndef _MINTSOUND_SOUND_SOURCE_SETTINGS_H_
#define _MINTSOUND_SOUND_SOURCE_SETTINGS_H_


#include "Common/Common.h"


namespace mint::sound
{

	struct SSoundSourceSettings
	{
		/// @brief -1 means loop forever, 0 means play once and 1..n means play n+1 times, i.e. 2 would play the sound 3 times.
		s8 m_loopMode = 0;

		f32 m_volume = 0.0f;

		f32 m_minDistance = 0.0f;

		f32 m_maxDistance = 0.0f;

		f32 m_pitch = 0.0f;

		f32 m_pan = 0.0f;

		u64 m_mode = FMOD_3D | FMOD_LOOP_NORMAL;

		Vec2 m_position = { 0.0f, 0.0f };

		Vec2 m_velocity = { 0.0f, 0.0f };

		/// @brief In which direction the sound cone is facing in 3D.
		Vec3 m_coneOrientation = { 0.0f, 0.0f, 0.0f };

		/// @brief Inner angle of the cone. From 0.0 to 360.0. Must be smaller than the outside cone angle.
		/// Outside angle of the cone. From 0.0 to 360.0.
		/// Volume outside of the cone. From 0.0 to 1.0.
		Vec3 m_coneSettings = { 360.0f, 360.0f, 1.0f };
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