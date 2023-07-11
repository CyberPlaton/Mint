#include "SoundSource.h"


namespace mint::sound
{

	CSoundSourceGroup* CSoundSource::get_sound_source_group() const
	{
		return m_group;
	}

	void CSoundSource::set_sound_source_group(CSoundSourceGroup* group)
	{
		m_group = group;
	}

	CSoundSource::CSoundSource() : 
		m_group(nullptr), m_channel(nullptr), m_sound(0)
	{

	}

	CSoundSource::~CSoundSource()
	{
		m_channel = nullptr;
		m_group = nullptr;
		m_sound = 0;
	}

	void CSoundSource::set_audio_source_file(SoundHandle handle)
	{
		m_sound = handle;
	}

	mint::SoundHandle CSoundSource::get_audio_source_file() const
	{                                                          
		return m_sound;
	}

	void CSoundSource::play_sound_source(FMOD::System* system, FMOD::Sound* sound)
	{
		// Assign a new channel for the sound source.                      
        system->playSound(sound, m_group->get_channel_group(), true, &m_channel);

		// Set settings for the sound source channel.
		m_channel->setMode(m_settings.m_mode);
		m_channel->setPitch(m_settings.m_pitch);
		m_channel->setPan(m_settings.m_pan);
		m_channel->setVolume(m_settings.m_volume);
		  
		FMOD_VECTOR fposition, fvelocity, forient;

		fposition.x = m_settings.m_position.x;
		fposition.y = m_settings.m_position.y;
		fposition.z = 0.0f;

		fvelocity.x = m_settings.m_velocity.x;
		fvelocity.y = m_settings.m_velocity.y;
		fvelocity.z = 0.0f;

		forient.x = m_settings.m_coneOrientation.x;
		forient.y = m_settings.m_coneOrientation.y;
		forient.z = 0.0f;

		m_channel->set3DAttributes(&fposition, &fvelocity);
		
		m_channel->set3DConeOrientation(&forient);

		m_channel->set3DConeSettings(m_settings.m_coneSettings.x, m_settings.m_coneSettings.y, m_settings.m_coneSettings.z);

		m_channel->setLoopCount(m_settings.m_loopMode);

		// Let the sound play.
		m_channel->setPaused(false);
	}


	void CSoundSource::stop_sound_source()
	{
		m_channel->stop();
	}

	void CSoundSource::set_mode(FMOD_MODE mode)
	{
		m_settings.m_mode = mode;
	}

	void CSoundSource::set_pitch(f32 value)
	{
		m_settings.m_pitch = value;
	}

	void CSoundSource::set_pan(f32 value)
	{
		m_settings.m_pan = value;
	}

	void CSoundSource::set_velocity(const Vec2& vec)
	{
		m_settings.m_velocity = vec;
	}

	void CSoundSource::set_position(const Vec2& vec)
	{
		m_settings.m_position = vec;
	}

	void CSoundSource::set_volume(f32 value)
	{
		m_settings.m_volume = value;
	}

	void CSoundSource::set_cone_orientation(const Vec3& vec)
	{
		m_settings.m_coneOrientation = vec;
	}

	void CSoundSource::set_cone_settings(f32 inner_cone_angle /*= 360.0f*/, f32 outer_cone_angle /*= 360.0f*/, f32 cone_outside_volume /*= 1.0f*/)
	{
		m_settings.m_coneSettings = { inner_cone_angle, outer_cone_angle, cone_outside_volume };
	}

	void CSoundSource::set_sound_handle(SoundHandle handle)
	{
		m_sound = handle;
	}

	void CSoundSource::pause_sound_source()
	{
		m_channel->setPaused(true);
	}

	FMOD::Channel* CSoundSource::get_channel() const
	{
		return m_channel;
	}

	bool CSoundSource::initialize(FMOD::System* system, FMOD::Sound* sound)
	{
		if (system->playSound(sound, m_group->get_channel_group(), true, &m_channel) == FMOD_OK)
		{
			return true;
		}

		MINT_LOG_ERROR("[{:.4f}][CSoundSource::initialize] Failed to initialize sound source \"{}\"!", MINT_APP_TIME, m_sound);
		MINT_ASSERT(false, "Invalid operation. Failed to initialize sound source!");
		return false;
	}

	void CSoundSource::terminate()
	{

	}

	void CSoundSource::play_sound_source_start_paused(FMOD::System* system, FMOD::Sound* sound)
	{
		FMOD_RESULT result;

		if (result = system->playSound(sound, m_group->get_channel_group(), true, &m_channel); result != FMOD_OK)
		{
			auto error = FMOD_ErrorString(result);
		
			MINT_LOG_ERROR("[{:.4f}][CSoundSource::play_sound_source_start_paused] Failed to play sound source. Error: \"{}\"!", MINT_APP_TIME, m_sound, error);
			MINT_ASSERT(false, "Invalid operation. Failed to play sound source!");
		}
	}

	bool CSoundSource::is_playing() const
	{
		bool result;
		if (m_channel->isPlaying(&result) == FMOD_OK)
		{
			return result;
		}

		return false;
	}

	glm::u32 CSoundSource::get_sound_position_minutes() const
	{
		u32 result = 0;
		if (is_playing())
		{
			m_channel->getPosition(&result, FMOD_TIMEUNIT_MS);

			return result / 1000 / 60;
		}

		return result;
	}

	glm::u32 CSoundSource::get_sound_position_seconds() const
	{
		u32 result = 0;
		if (is_playing())
		{
			m_channel->getPosition(&result, FMOD_TIMEUNIT_MS);

			return result / 1000 % 60;
		}

		return result;
	}

	glm::u32 CSoundSource::get_sound_position_milliseconds() const
	{
		u32 result = 0;
		if (is_playing())
		{
			m_channel->getPosition(&result, FMOD_TIMEUNIT_MS);

			return result / 10 % 100;
		}

		return result;
	}

}
