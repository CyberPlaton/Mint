#include "SoundSystem.h"


namespace mint::sound
{
	bool CSoundEngine::add_event_listener(const String& event_name)
	{
		auto h = mint::algorithm::djb_hash(event_name);

		if (!m_delegates.lookup(h))
		{
			auto delegate = new SDelegate(event_name, event_name + String("_Delegate"));

			delegate->set_execute_function(mint::sound::detail::propagate_received_event);

			m_delegates.add(h, delegate);

			CEventSystem::Get().add_listener(event_name, delegate);

			return true;
		}

		return false;
	}

	void CSoundEngine::propagate_received_event(SEvent* event)
	{
		// Receive incoming event.
		m_currentEventCursor = (m_currentEventCursor + 1) % MINTSOUND_INCOMING_EVENT_COUNT_MAX;

		// Create a copy of the event for later processing.
		auto& copy = m_incomingEvents[m_currentEventCursor];

		copy.copy_from(event);
	}

	void CSoundEngine::remove_event_listener(const String& event_name)
	{
		auto h = mint::algorithm::djb_hash(event_name);

		if (m_delegates.lookup(h))
		{
			m_delegates.remove(h);

			auto id = mint::algorithm::djb_hash(event_name + String("_Delegate"));

			CEventSystem::Get().remove_listener(event_name, id);
		}
	}

	void CSoundEngine::reset()
	{
		// Unregister delegates from event system.
		// Note: This may be redundant, as the event system is doing the same on reset.
		for (auto delegate : m_delegates.get_all_const())
		{
			CEventSystem::Get().remove_listener(delegate->get_listening_event_type(), delegate->get_unique_identifier());
		}

		// We are responsible for freeing used memory from delegates.
		auto& delegates = m_delegates.get_all();
		while (!delegates.empty())
		{
			auto d = delegates[0];

			delete d; d = nullptr;

			delegates.erase(delegates.begin());
		}
	}

	bool CSoundEngine::initialize()
	{
		FMOD_RESULT result;

		FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_LOG);

		if (result = FMOD::System_Create(&m_system); result == FMOD_OK)
		{
			if (result = m_system->init(32, /*FMOD_INIT_3D_RIGHTHANDED*/ FMOD_INIT_NORMAL, nullptr); result == FMOD_OK)
			{
				m_system->set3DSettings(1.0f, 1.0f, 1.0f);

				m_incomingEvents.resize(MINTSOUND_INCOMING_EVENT_COUNT_MAX);

				FMOD::ChannelGroup* root = nullptr;

				m_system->getMasterChannelGroup(&root);
				m_root.set_channel_group(root);
				m_root.set_group_name("Master");
				
				return true;
			}
		}
		
		auto error = FMOD_ErrorString(result);

		MINT_LOG_ERROR("[{:.4f}][CSoundEngine::initialize] Failed to initialize FMOD: \"{}\"!", MINT_APP_TIME, error);

		return false;
	}

	void CSoundEngine::terminate()
	{
		for (auto sound : m_sounds.get_all())
		{
			sound->release();
		}

		m_incomingEvents.clear();
		m_system->close();
		m_system->release();
	}

	FMOD::Sound* CSoundEngine::_get_sound(const String& sound_name)
	{
		auto h = mint::algorithm::djb_hash(sound_name);

		return _get_sound(h);
	}

	FMOD::Sound* CSoundEngine::_get_sound(SoundHandle handle)
	{
		if (m_sounds.lookup(handle))
		{
			return m_sounds.get(handle);
		}

		MINT_LOG_ERROR("[{:.4f}][CSoundEngine::_get_sound] Failed locating sound \"{}\"!", MINT_APP_TIME, handle);

		return nullptr;
	}

	bool CSoundEngine::_load_sound(const String& sound_name)
	{
		auto h = mint::algorithm::djb_hash(sound_name);

		return _load_sound(h);
	}

	bool CSoundEngine::_load_sound(SoundHandle handle)
	{
		if (m_soundPrefabs.lookup(handle))
		{
			auto& pair = m_soundPrefabs.get_ref(handle);

			FMOD::Sound* sound = nullptr;
			
			auto result = m_system->createSound(pair.second.c_str(), FMOD_3D | FMOD_CREATESAMPLE, 0, &sound);

			if (result != FMOD_OK)
			{
				MINT_LOG_ERROR("[{:.4f}][CSoundEngine::_load_sound] Failed loading sound \"{}\" at \"{}\"!", MINT_APP_TIME, pair.first, pair.second);
				return false;
			}

			sound->setMode(FMOD_3D);
			
			m_sounds.add(handle, sound);

			return true;
		}

		MINT_LOG_ERROR("[{:.4f}][CSoundEngine::_load_sound] Failed locating sound prefab \"{}\"!", MINT_APP_TIME, handle);

		return false;
	}

	void CSoundEngine::create_sound_prefab(const String& sound_name, const String& sound_file_path)
	{
		auto h = mint::algorithm::djb_hash(sound_name);

		m_soundPrefabs.add(h, std::make_pair(sound_name, sound_file_path));
	}

	void CSoundEngine::remove_sound_source(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			m_soundSources.erase(h);
		}
	}

	void CSoundEngine::create_sound_source(entt::entity entity, const String& sound_name)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) == m_soundSources.end())
		{
			auto sh = mint::algorithm::djb_hash(sound_name);

			if (!m_sounds.lookup(sh) && !_load_sound(sh))
			{
				MINT_LOG_ERROR("[{:.4f}][CSoundEngine::create_sound_source] Sound \"{}\" could not be loaded for entity \"{}\"!", MINT_APP_TIME, sound_name, h);
				MINT_ASSERT(false, "Invalid operation. Failed to load sound source!");
				return;
			}

			auto& source = m_soundSources[h];

			source.set_audio_source_file(sh);
			source.set_sound_source_group(&m_root);

			if (!source.initialize(m_system, m_sounds.get(sh)))
			{
				MINT_LOG_ERROR("[{:.4f}][CSoundEngine::create_sound_source] Sound \"{}\" could not be initialized for entity \"{}\"!", MINT_APP_TIME, sound_name, h);
				MINT_ASSERT(false, "Invalid operation. Failed to initialize sound source!");
			}

			CUCA::soundsource_set_sound_source_sound_handle(entity, sh);
			
			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::create_sound_source] Sound source does already exist for entity \"{}\" with sound name \"{}\"!", MINT_APP_TIME, h, sound_name);
	}

	void CSoundEngine::on_update(f32 dt)
	{
		MINT_PROFILE_SCOPE("Engine::Sound", "CSoundEngine::on_update");

		_set_listener_data(m_listenerPosition, m_listenerVelocity, m_listenerForward, m_listenerUp);

		auto& view = MINT_SCENE_REGISTRY()->get_registry_view< mint::component::SSoundSource >();

		for (auto& entity : view)
		{
			if (CUCA::soundsource_get_sound_source_3d_to_2d_morphing_enabled(entity))
			{
				auto& source = m_soundSources[SCAST(u64, entity)];

				auto position = CUCA::transform_get_position(entity);

				auto dist = mint::algorithm::vec2_compute_distance_between_two_vectors(m_listenerPosition, position);

				if (dist < m_morphingThreshold)
				{
					auto p = mint::algorithm::compute_percent_value(m_morphingThreshold, dist);

					// Value is always between 0.0f(2D) and 1.0f(3D).
					source.get_channel()->set3DLevel(p / 100.0f);
				}
				else
				{
					source.get_channel()->set3DLevel(1.0f);
				}
			}
		}


		m_system->update();
	}

	bool CSoundEngine::create_sound_source_group(const String& group_name, const String& parent_group /*= "Master"*/)
	{
		auto h = mint::algorithm::djb_hash(group_name);
		auto ph = mint::algorithm::djb_hash(parent_group);

		if (!m_channelGroups.lookup(h))
		{
			CSoundSourceGroup ssgroup(group_name);

			// Try creating the channel group.
			FMOD::ChannelGroup* channel_group = nullptr;

			if (m_system->createChannelGroup(group_name.c_str(), &channel_group) == FMOD_OK)
			{
				ssgroup.set_channel_group(channel_group);
			}
			else
			{
				return false;
			}


			if (ph == m_root.get_group_identifier())
			{
				ssgroup.set_parent(ph);
				m_root.add_child_group(h, channel_group);
			}
			else if (m_channelGroups.lookup(ph))
			{
				auto& pgroup = m_channelGroups.get_ref(ph);

				pgroup.add_child_group(h, channel_group);
			}
			else
			{
				// Set Master as parent forcefully.
				MINT_LOG_WARN("[{:.4f}][CSoundEngine::create_sound_source_group] Sound source group \"{}\" could not be found, setting \"Master\" as parent for \"{}\" forcefully!", MINT_APP_TIME, parent_group, group_name);
				ssgroup.set_parent(ph);
				m_root.add_child_group(h, channel_group);
			}

			return true;
		}

		return false;
	}

	void CSoundEngine::set_sound_source_group_settings(const String& group_name, const SSoundSourceGroupSettings& settings)
	{
		auto h = mint::algorithm::djb_hash(group_name);

		if (h == m_root.get_group_identifier())
		{
			m_root.set_sound_group_settings(settings);
			m_root.apply_sound_group_settings();
		}
		else if (m_channelGroups.lookup(h))
		{
			auto& group = m_channelGroups.get_ref(h);

			group.set_sound_group_settings(settings);
			group.apply_sound_group_settings();
		}
		else
		{
			MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_group_settings] Sound source group \"{}\" could not be found!", MINT_APP_TIME, group_name);
			MINT_ASSERT(false, "Invalid operation. Sound source group does not exist!");
		}
	}

	void CSoundEngine::set_sound_source_mode(entt::entity entity, FMOD_MODE mode)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			source.set_mode(mode);

			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_mode] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}

	void CSoundEngine::set_sound_source_pitch(entt::entity entity, f32 value)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			source.set_pitch(value);

			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_pitch] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}

	void CSoundEngine::set_sound_source_min_and_max_distance(entt::entity entity, f32 min, f32 max)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			source.set_min_and_max_distance(min, max);

			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_min_and_max_distance] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}

	void CSoundEngine::set_sound_source_pan(entt::entity entity, f32 value)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			source.set_pan(value);

			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_pan] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}

	void CSoundEngine::set_sound_source_volume(entt::entity entity, f32 value)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			source.set_volume(value);

			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_volume] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}

	void CSoundEngine::set_sound_source_velocity(entt::entity entity, const Vec3& vec)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			auto pos = CUCA::transform_get_position(entity);
			auto height = CUCA::soundsource_get_sound_source_height(entity);

			source.set_velocity_and_position(vec, { pos.x, pos.y, height});

			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_velocity] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}

	void CSoundEngine::set_sound_source_position(entt::entity entity, const Vec3& vec)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			source.set_velocity_and_position(CUCA::soundsource_get_sound_source_velocity(entity), vec);

			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_position] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}


	void CSoundEngine::set_sound_source_position_and_velocity(entt::entity entity, const Vec3& position, const Vec3& velocity)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			source.set_velocity_and_position(velocity, position);

			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_position_and_velocity] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}

	void CSoundEngine::set_sound_source_sound_handle(entt::entity entity, SoundHandle handle)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			source.set_sound_handle(handle);

			return;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::set_sound_source_sound_handle] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}


	void CSoundEngine::_set_listener_data(const Vec3& position, const Vec3& velocity, const Vec3& forward, const Vec3& up)
	{
		m_listenerPosition = position;
		m_listenerVelocity = velocity;
		m_listenerForward = forward;
		m_listenerUp = up;

		FMOD_VECTOR p, v, f, u;

		p.x = position.x;
		p.y = position.y;
		p.z = position.z;

		v.x = velocity.x;
		v.y = velocity.y;
		v.z = velocity.z;

		f.x = forward.x;
		f.y = forward.y;
		f.z = forward.z;

		u.x = up.x;
		u.y = up.y;
		u.z = up.z;

		m_system->set3DListenerAttributes(0, &p, &v, &f, &u);
	}

	mint::Vec3 CSoundEngine::get_listener_position() const
	{
		return m_listenerPosition;
	}

	mint::Vec3 CSoundEngine::get_listener_velocity() const
	{
		return m_listenerVelocity;
	}

	mint::Vec3 CSoundEngine::get_listener_forward() const
	{
		return m_listenerForward;
	}

	mint::Vec3 CSoundEngine::get_listener_up() const
	{
		return m_listenerUp;
	}

	void CSoundEngine::_check_fmod_error(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			auto error = FMOD_ErrorString(result);
			MINT_LOG_WARN("[{:.4f}][CSoundEngine::_check_fmod_error] FMOD error encountered: \"{}\"!", MINT_APP_TIME, error);
			MINT_ASSERT(false, "Invalid operation. FMOD error!");
		}
	}

	mint::u32 CSoundEngine::get_sound_length(const String& sound_name)
	{
		auto h = mint::algorithm::djb_hash(sound_name);

		u32 result = 0;

		if (m_sounds.lookup(h))
		{
			auto sound = m_sounds.get(h);

			sound->getLength(&result, FMOD_TIMEUNIT_MS);

			return result;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length] Sound source with name \"{}\" could not be located!", MINT_APP_TIME, sound_name);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return result;
	}

	mint::u32 CSoundEngine::get_sound_length(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		u32 result = 0;

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			auto sound = _get_sound(source.get_audio_source_file());

			sound->getLength(&result, FMOD_TIMEUNIT_MS);

			return result;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}

	mint::u32 CSoundEngine::get_sound_position(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			if (source.is_playing())
			{
				return source.get_sound_position();
			}
			else return 0;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_position] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return 0;
	}

	glm::u32 CSoundEngine::get_sound_length_minutes(const String& sound_name)
	{
		auto h = mint::algorithm::djb_hash(sound_name);

		u32 result = 0;

		if (m_sounds.lookup(h))
		{
			auto sound = m_sounds.get(h);

			sound->getLength(&result, FMOD_TIMEUNIT_MS);

			return result / 1000 / 60;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length_minutes] Sound source with name \"{}\" could not be located!", MINT_APP_TIME, sound_name);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return result;
	}

	glm::u32 CSoundEngine::get_sound_length_seconds(const String& sound_name)
	{
		auto h = mint::algorithm::djb_hash(sound_name);

		u32 result = 0;

		if (m_sounds.lookup(h))
		{
			auto sound = m_sounds.get(h);

			sound->getLength(&result, FMOD_TIMEUNIT_MS);

			return result / 1000 % 60;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length_seconds] Sound source with name \"{}\" could not be located!", MINT_APP_TIME, sound_name);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return result;
	}

	glm::u32 CSoundEngine::get_sound_length_milliseconds(const String& sound_name)
	{
		auto h = mint::algorithm::djb_hash(sound_name);

		u32 result = 0;

		if (m_sounds.lookup(h))
		{
			auto sound = m_sounds.get(h);

			sound->getLength(&result, FMOD_TIMEUNIT_MS);

			return result / 10 % 100;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length_milliseconds] Sound source with name \"{}\" could not be located!", MINT_APP_TIME, sound_name);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return result;
	}

	glm::u32 CSoundEngine::get_sound_length_minutes(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		u32 result = 0;

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			auto sound = _get_sound(source.get_audio_source_file());

			sound->getLength(&result, FMOD_TIMEUNIT_MS);

			return result / 1000 / 60;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length_minutes] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");
	}

	glm::u32 CSoundEngine::get_sound_length_seconds(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		u32 result = 0;

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			auto sound = _get_sound(source.get_audio_source_file());

			sound->getLength(&result, FMOD_TIMEUNIT_MS);

			return result / 1000 % 60;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length_seconds] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return result;
	}

	glm::u32 CSoundEngine::get_sound_length_milliseconds(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		u32 result = 0;

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			auto sound = _get_sound(source.get_audio_source_file());

			sound->getLength(&result, FMOD_TIMEUNIT_MS);

			return result / 10 % 100;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length_milliseconds] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return result;
	}

	glm::u32 CSoundEngine::get_sound_position_minutes(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			if (source.is_playing())
			{
				return source.get_sound_position_minutes();
			}
			else return 0;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length_milliseconds] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return 0;
	}

	glm::u32 CSoundEngine::get_sound_position_seconds(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			if (source.is_playing())
			{
				return source.get_sound_position_seconds();
			}
			else return 0;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length_milliseconds] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return 0;
	}

	glm::u32 CSoundEngine::get_sound_position_milliseconds(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			if (source.is_playing())
			{
				return source.get_sound_position_milliseconds();
			}
			else return 0;
		}

		MINT_LOG_WARN("[{:.4f}][CSoundEngine::get_sound_length_milliseconds] Sound source for entity \"{}\" could not be located!", MINT_APP_TIME, h);
		MINT_ASSERT(false, "Invalid operation. Sound source could not be located!");

		return 0;
	}

	void CSoundEngine::play_sound(entt::entity entity)
	{
		// Play only if the sound source is enabled.
		if (CUCA::world_get_entity_enabled(entity))
		{
			auto h = SCAST(u64, entity);
			auto sh = CUCA::soundsource_get_sound_source_sound_handle(entity);

			if (m_soundSources.find(h) != m_soundSources.end() && m_sounds.lookup(sh))
			{
				auto& source = m_soundSources[h];
				auto sound = m_sounds.get(sh);

				m_system->playSound(sound, 0, true, &source.m_channel);

				// Set updated data for the channel.
				//source.m_channel->setMode(FMOD_3D);

				auto pos = CUCA::transform_get_position(entity);
				FMOD_MODE mode = CUCA::soundsource_get_sound_source_mode(entity);

				source.set_mode(FMOD_3D);
				source.set_velocity_and_position(CUCA::soundsource_get_sound_source_velocity(entity), { pos.x, pos.y, CUCA::soundsource_get_sound_source_height(entity) });

				//source.m_channel->setPriority(1);

				source.set_pitch(CUCA::soundsource_get_sound_source_pitch(entity));
				source.set_pan(CUCA::soundsource_get_sound_source_pan(entity));
				source.set_volume(CUCA::soundsource_get_sound_source_volume(entity));
				source.set_loop_mode(CUCA::soundsource_get_sound_source_loopmode(entity));
				source.set_min_and_max_distance(CUCA::soundsource_get_sound_source_min_distance(entity), CUCA::soundsource_get_sound_source_max_distance(entity));


				source.play_sound_source();

				//if (source.is_virtual())
				//{
					//source.stop_sound_source();
				//}

				//MINT_ASSERT(source.is_playing() == true, "Invalid operation. Failed to play sound source!");
				//MINT_ASSERT(source.is_virtual() == false, "Invalid operation. Sound source was virtualized and is not outputting sound!");
			}
		}
	}

	bool CSoundEngine::is_sound_source_virtual(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			return source.is_virtual();
		}

		return false;
	}

	mint::u32 CSoundEngine::get_sound_source_size_in_bytes(SoundHandle handle)
	{
		if (m_sounds.lookup(handle))
		{
			auto sound = m_sounds.get(handle);
			
			u32 result = 0;

			_check_fmod_error(sound->getLength(&result, FMOD_TIMEUNIT_RAWBYTES));

			return result;
		}

		return 0;
	}

	bool CSoundEngine::get_sound_source_data(SoundHandle handle, void* buffer, u32* buffer_size)
	{
		if (m_sounds.lookup(handle))
		{
			// Load sound for reading data and close afterwards. Easier than to synchronize with FMOD and engine.
			auto& pair = m_soundPrefabs.get_ref(handle);

			FMOD::Sound* sound = nullptr;

			_check_fmod_error(m_system->createSound(pair.second.c_str(), FMOD_DEFAULT | FMOD_OPENONLY, 0, &sound));

			u32 length = 0;

			sound->getLength(&length, FMOD_TIMEUNIT_RAWBYTES);

			_check_fmod_error(sound->readData(buffer, length, buffer_size));

			return true;
		}

		return false;
	}

	void CSoundEngine::stop_sound(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			if (source.is_playing())
			{
				source.get_channel()->stop();
			}
		}
	}

	void CSoundEngine::toggle_pause_sound(entt::entity entity)
	{
		auto h = SCAST(u64, entity);

		if (m_soundSources.find(h) != m_soundSources.end())
		{
			auto& source = m_soundSources[h];

			if (source.is_paused() == false)
			{
				source.pause_sound_source();
			}
			else source.resume_playing_sound_source();
			
		}
	}

	void CSoundEngine::set_listener_position(const Vec3& vec)
	{
		m_listenerPosition = vec;
	}

	void CSoundEngine::set_listener_velocity(const Vec3& vec)
	{
		m_listenerVelocity = vec;
	}

	void CSoundEngine::set_listener_forward(const Vec3& vec)
	{
		m_listenerForward = vec;
	}

	void CSoundEngine::set_listener_up(const Vec3& vec)
	{
		m_listenerUp = vec;
	}

	void CSoundEngine::set_3d_to_2d_morphing_threshold(f32 value)
	{
		m_morphingThreshold = value;
	}

	mint::f32 CSoundEngine::get_3d_to_2d_morphing_threshold() const
	{
		return m_morphingThreshold;
	}


	namespace detail
	{
		void propagate_received_event(SEvent* event)
		{
			CSoundEngine::Get().propagate_received_event(event);
		}
	}
	
}

