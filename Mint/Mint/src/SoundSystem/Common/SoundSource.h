#ifndef _MINTSOUND_SOUND_SOURCE_H_
#define _MINTSOUND_SOUND_SOURCE_H_


#include "SoundSourceSettings.h"
#include "SoundSourceGroup.h"
#include "Utility/Logging/Logging.h"


namespace mint::sound
{
	class CSoundEngine;


	/// @brief Representation of a sound in the game. Does play some audio file and has
	/// attributes such volume, position and velocity in 3D space. The position of the listener, i.e.
	/// the player character or the camera does affect those values, reducing/increasing the volume, the 
	/// speaker placement and pitch of the final sound palyed.
	/// @reference FMOD::Channel.
	class CSoundSource
	{
		friend class CSoundEngine;
	public:
		CSoundSource();
		virtual ~CSoundSource();

		bool initialize(FMOD::System* system, FMOD::Sound* sound);

		void terminate();


		void set_audio_source_file(SoundHandle handle);

		SoundHandle get_audio_source_file() const;

		
		void start_sound_source_paused(FMOD::System* system, FMOD::Sound* sound);

		void play_sound_source();

		void resume_playing_sound_source();

		bool is_playing() const;

		bool is_paused() const;

		bool is_virtual() const;

		void pause_sound_source();

		void stop_sound_source();



		u32 get_sound_position_minutes() const;

		u32 get_sound_position_seconds() const;

		u32 get_sound_position_milliseconds() const;

		u32 get_sound_position() const;


		FMOD::Channel* get_channel() const;

		CSoundSourceGroup* get_sound_source_group() const;

		void set_sound_source_group(CSoundSourceGroup* group);

		void set_loop_mode(bool mode);
		void set_mode(FMOD_MODE mode);
		void set_min_and_max_distance(f32 min, f32 max);
		void set_sound_handle(SoundHandle handle);
		void set_pitch(f32 value);
		void set_pan(f32 value);
		void set_volume(f32 value);
		void set_velocity_and_position(const Vec3& velocity, const Vec3& position);


	protected:
		FMOD::Channel* m_channel;

		SoundHandle m_sound;

		CSoundSourceGroup* m_group;


	protected:
		void play_sound_source_start_paused(FMOD::System* system, FMOD::Sound* sound);

	private:
		void _check_fmod_error(FMOD_RESULT result) const;
	};
}

#endif