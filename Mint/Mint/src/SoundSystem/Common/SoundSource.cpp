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

	void CSoundSource::start_sound_source_paused(FMOD::System* system, FMOD::Sound* sound)
	{
		// Assign a new channel for the sound source.                      
		_check_fmod_error( system->playSound(sound, m_group->get_channel_group(), true, &m_channel) );
	}

	void CSoundSource::play_sound_source()
	{
		_check_fmod_error(m_channel->setPaused(false));
	}

	void CSoundSource::stop_sound_source()
	{
		_check_fmod_error(m_channel->stop());
	}

	void CSoundSource::set_mode(FMOD_MODE mode)
	{
		_check_fmod_error(m_channel->setMode(mode));
	}

	void CSoundSource::set_pitch(f32 value)
	{
		_check_fmod_error(m_channel->setPitch(value));
	}

	void CSoundSource::set_pan(f32 value)
	{
		_check_fmod_error(m_channel->setPan(value));
	}

	void CSoundSource::set_velocity_and_position(const Vec3& velocity, const Vec3& position)
	{
		FMOD_VECTOR fposition, fvelocity;

		fposition.x = position.x;
		fposition.y = position.y;
		fposition.z = position.z;

		fvelocity.x = velocity.x;
		fvelocity.y = velocity.y;
		fvelocity.z = velocity.z;

		_check_fmod_error(m_channel->set3DAttributes(&fposition, &fvelocity));
	}

	void CSoundSource::set_volume(f32 value)
	{
		_check_fmod_error(m_channel->setVolume(value));
	}

	void CSoundSource::set_cone_orientation(const Vec3& vec)
	{
		FMOD_VECTOR forient;

		forient.x = vec.x;
		forient.y = vec.y;
		forient.z = vec.z;

		_check_fmod_error(m_channel->set3DConeOrientation(&forient));
	}

	void CSoundSource::set_cone_settings(f32 inner_cone_angle /*= 360.0f*/, f32 outer_cone_angle /*= 360.0f*/, f32 cone_outside_volume /*= 1.0f*/)
	{
		_check_fmod_error(m_channel->set3DConeSettings(inner_cone_angle, outer_cone_angle, cone_outside_volume));
	}

	void CSoundSource::set_sound_handle(SoundHandle handle)
	{
		m_sound = handle;
	}

	void CSoundSource::pause_sound_source()
	{
		_check_fmod_error(m_channel->setPaused(true));
	}

	FMOD::Channel* CSoundSource::get_channel() const
	{
		return m_channel;
	}

	bool CSoundSource::initialize(FMOD::System* system, FMOD::Sound* sound)
	{
		return true;
	}

	void CSoundSource::terminate()
	{

	}

	void CSoundSource::play_sound_source_start_paused(FMOD::System* system, FMOD::Sound* sound)
	{
		FMOD_RESULT result;

		_check_fmod_error(system->playSound(sound, m_group->get_channel_group(), true, &m_channel));
	}

	bool CSoundSource::is_playing() const
	{
		bool result;

		if (m_channel != nullptr)
		{
			_check_fmod_error(m_channel->isPlaying(&result));

			return result;
		}

		return false;
	}

	bool CSoundSource::is_virtual() const
	{
		bool result;
		
		if (m_channel != nullptr)
		{
			_check_fmod_error(m_channel->isVirtual(&result));

			return result;
		}

		return false;
	}

	glm::u32 CSoundSource::get_sound_position_minutes() const
	{
		u32 result = 0;
		if (is_playing())
		{
			_check_fmod_error(m_channel->getPosition(&result, FMOD_TIMEUNIT_MS));

			return result / 1000 / 60;
		}

		return result;
	}

	glm::u32 CSoundSource::get_sound_position_seconds() const
	{
		u32 result = 0;
		if (is_playing())
		{
			_check_fmod_error(m_channel->getPosition(&result, FMOD_TIMEUNIT_MS));

			return result / 1000 % 60;
		}

		return result;
	}

	glm::u32 CSoundSource::get_sound_position_milliseconds() const
	{
		u32 result = 0;
		if (is_playing())
		{
			_check_fmod_error(m_channel->getPosition(&result, FMOD_TIMEUNIT_MS));

			return result / 10 % 100;
		}

		return result;
	}

	void CSoundSource::set_loop_mode(s8 mode)
	{
		_check_fmod_error(m_channel->setLoopCount(mode));
	}

	void CSoundSource::_check_fmod_error(FMOD_RESULT result) const
	{
		if (!(result == FMOD_OK || result == FMOD_ERR_CHANNEL_STOLEN ||
			  result == FMOD_ERR_INVALID_HANDLE))
		{
			auto error = FMOD_ErrorString(result);
			MINT_LOG_WARN("[{:.4f}][CSoundSource::_check_fmod_error] FMOD error encountered: \"{}\"!", MINT_APP_TIME, error);
			MINT_ASSERT(false, "Invalid operation. FMOD error!");
		}
	}

	void CSoundSource::resume_playing_sound_source()
	{
		if (is_paused())
		{
			_check_fmod_error(m_channel->setPaused(false));
		}
	}

	bool CSoundSource::is_paused() const
	{
		bool result;

		if (m_channel != nullptr)
		{
			_check_fmod_error(m_channel->getPaused(&result));

			return result;
		}

		return false;
	}

	void CSoundSource::set_min_and_max_distance(f32 min, f32 max)
	{
		_check_fmod_error(m_channel->set3DMinMaxDistance(min, max));
	}

}
