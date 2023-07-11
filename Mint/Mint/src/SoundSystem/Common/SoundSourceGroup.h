#ifndef _MINTSOUND_SOUND_SOURCE_GROUP_H_
#define _MINTSOUND_SOUND_SOURCE_GROUP_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "SoundSourceSettings.h"


namespace mint::sound
{
	/// @brief Collection of sound sources. Allows to set attributes for a set of
	/// sound sources collectively at once, such as 
	/// This is a hierarchical structure, it has a set of children sound source groups and one parent or none.
	/// Attributes set for this collection apply to all children recursively.
	/// @reference FMOD::ChannelGroup.
	class CSoundSourceGroup
	{
	public:
		CSoundSourceGroup();
		CSoundSourceGroup(const String& group_name);
		virtual ~CSoundSourceGroup();

		void set_group_name(const String& group_name);

		String get_group_name() const;

		u64 get_group_identifier() const;


		void apply_sound_group_settings();
		
		void set_sound_group_settings(const SSoundSourceGroupSettings& settings);

		SSoundSourceGroupSettings get_sound_group_settings() const;


		bool has_parent() const;

		void set_parent(u64 parent_identifier);


		bool has_children() const;

		u64 get_parent_group() const;

		void add_child_group(u64 child_identifier, FMOD::ChannelGroup* child_group);

		void remove_child(u64 child_identifier);

		Vector< u64 > get_children() const;


		FMOD::ChannelGroup* get_channel_group() const;
	
		void set_channel_group(FMOD::ChannelGroup* group);




	protected:
		String m_groupName;

		FMOD::ChannelGroup* m_group;

		SSoundSourceGroupSettings m_settings;

		u64 m_parent;

		Vector< u64 > m_children;
	};
}

#endif