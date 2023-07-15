#ifndef _MINT_SOUND_SYSTEM_H_
#define _MINT_SOUND_SYSTEM_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Utility/STL/Map.h"
#include "Utility/EventSystem/EventSystem.h"
#include "Utility/Logging/Logging.h"
#include "Utility/Profiling/FunctionProfiler.h"

#include "Common/SoundSourceGroup.h"
#include "Common/SoundSourceSettings.h"
#include "Common/SoundSource.h"

#include "Components/CUCA.h"


namespace mint::sound
{

	class CSoundEngine
	{
	public:
		STATIC_GET(CSoundEngine, s_CSoundEngine);

		bool initialize();

		void terminate();

		void reset();


		void on_update(f32 dt);


		void set_listener_position(const Vec3& vec);
		void set_listener_velocity(const Vec3& vec);
		void set_listener_forward(const Vec3& vec);
		void set_listener_up(const Vec3& vec);

		Vec3 get_listener_position() const;
		Vec3 get_listener_velocity() const;
		Vec3 get_listener_forward() const;
		Vec3 get_listener_up() const;


		bool create_sound_source_group(const String& group_name, const String& parent_group = "Master");

		void set_sound_source_group_settings(const String& group_name, const SSoundSourceGroupSettings& settings);


		void play_sound(entt::entity entity);

		void toggle_pause_sound(entt::entity entity);

		void stop_sound(entt::entity entity);


		void create_sound_source(entt::entity entity, const String& sound_name);

		void remove_sound_source(entt::entity entity);


		u32 get_sound_length_minutes(const String& sound_name);

		u32 get_sound_length_seconds(const String& sound_name);

		u32 get_sound_length_milliseconds(const String& sound_name);

		u32 get_sound_length_minutes(entt::entity entity);

		u32 get_sound_length_seconds(entt::entity entity);

		u32 get_sound_length_milliseconds(entt::entity entity);

		u32 get_sound_position_minutes(entt::entity entity);
		
		u32 get_sound_position_seconds(entt::entity entity);
		
		u32 get_sound_position_milliseconds(entt::entity entity);



		bool add_event_listener(const String& event_name);

		void remove_event_listener(const String& event_name);

		void propagate_received_event(SEvent* event);


		void set_sound_source_sound_handle(entt::entity entity, SoundHandle handle);
		void set_sound_source_mode(entt::entity entity, FMOD_MODE mode);
		void set_sound_source_pitch(entt::entity entity, f32 value);
		void set_sound_source_min_and_max_distance(entt::entity entity, f32 min, f32 max);
		void set_sound_source_pan(entt::entity entity, f32 value);
		void set_sound_source_volume(entt::entity entity, f32 value);
		void set_sound_source_velocity(entt::entity entity, const Vec3& vec);
		void set_sound_source_position(entt::entity entity, const Vec3& vec);
		void set_sound_source_position_and_velocity(entt::entity entity, const Vec3& position, const Vec3& velocity);
		void set_sound_source_cone_orientation(entt::entity entity, const Vec3& vec);
		void set_sound_source_cone_settings(entt::entity entity, f32 inner_cone_angle = 360.0f, f32 outer_cone_angle = 360.0f, f32 cone_outside_volume = 1.0f);

		bool is_sound_source_virtual(entt::entity entity);


		void create_sound_prefab(const String& sound_name, const String& sound_file_path);


	private:
		FMOD::System* m_system = nullptr;

		Vec3 m_listenerPosition;
		Vec3 m_listenerVelocity;
		Vec3 m_listenerForward;
		Vec3 m_listenerUp;



		CMap< SDelegate* > m_delegates;

		Vector< SEvent > m_incomingEvents;
		
		u32 m_currentEventCursor;



		CSoundSourceGroup m_root;

		CMap< CSoundSourceGroup > m_channelGroups;



		std::unordered_map< u64, CSoundSource > m_soundSources;

		CMap< FMOD::Sound* > m_sounds;
		CMap< std::pair< String, String > > m_soundPrefabs;


	private:
		void _set_listener_data(const Vec3& position, const Vec3& velocity, const Vec3& forward, const Vec3& up);

		bool _load_sound(const String& sound_name);
		bool _load_sound(SoundHandle handle);

		FMOD::Sound* _get_sound(const String& sound_name);
		FMOD::Sound* _get_sound(SoundHandle handle);
	
		void _check_fmod_error(FMOD_RESULT result);
	};



	namespace detail
	{
		void propagate_received_event(SEvent* event);
	}
}


#endif