#ifndef _MINTSOUND_SOUND_SOURCE_H_
#define _MINTSOUND_SOUND_SOURCE_H_


#include "Common/Common.h"
#include "SoundCommon.h"


namespace mint::sound
{
	class CSoundSourceGroup;


	/// @brief Representation of a sound in the game. Does play some audio file and has
	/// attributes such volume, position and velocity in 3D space. The position of the listener, i.e.
	/// the player character or the camera does affect those values, reducing/increasing the volume, the 
	/// speaker placement and pitch of the final sound palyed.
	/// @reference FMOD::Channel.
	class CSoundSource
	{
	public:
		CSoundSource();
		virtual ~CSoundSource();

		virtual bool initialize() { MINT_ASSERT(false, "Invalid operation. Using interface class is not allowed!"); return false; };

		virtual void terminate() {};


		virtual bool set_audio_source_file(const String& full_file_path) { return false; };

		virtual String get_audio_source_file() const { return ""; };

		CSoundSourceGroup* get_sound_source_group() const;

		void set_sound_source_group(CSoundSourceGroup* group);



		f32 get_volume() const;

		void set_volume(f32 value);

		Vec2 get_velocity() const;

		void set_velocity(const Vec2& vec);

		Vec2 get_position() const;

		void set_position(const Vec2& vec);


	protected:
		f32 m_volume;
		Vec2 m_position;
		Vec2 m_velocity;

		CSoundSourceGroup* m_group;

	};
}

#endif