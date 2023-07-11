#include "SoundSourceGroup.h"


namespace mint::sound
{

	void CSoundSourceGroup::apply_sound_group_settings()
	{
		m_group->setVolume(m_settings.m_volume);
		m_group->setPitch(m_settings.m_pitch);
		m_group->setPan(m_settings.m_pan);
		m_group->setMode(m_settings.m_mode);
	}

	FMOD::ChannelGroup* CSoundSourceGroup::get_channel_group() const
	{
		return m_group;
	}

	CSoundSourceGroup::CSoundSourceGroup() : 
		m_group(nullptr), m_parent(0)
	{
	}

	CSoundSourceGroup::CSoundSourceGroup(const String& group_name) :
		m_group(nullptr), m_parent(0), m_groupName(group_name)
	{
	}

	CSoundSourceGroup::~CSoundSourceGroup()
	{

	}

	void CSoundSourceGroup::set_channel_group(FMOD::ChannelGroup* group)
	{
		m_group = group;
	}

	bool CSoundSourceGroup::has_parent() const
	{
		return m_parent != 0;
	}

	void CSoundSourceGroup::set_parent(u64 parent_identifier)
	{
		m_parent = parent_identifier;
	}

	bool CSoundSourceGroup::has_children() const
	{
		return !m_children.empty();
	}

	void CSoundSourceGroup::add_child_group(u64 child_identifier, FMOD::ChannelGroup* child_group)
	{
		mint::algorithm::vector_push_back(m_children, child_identifier);

		m_group->addGroup(child_group);
	}

	void CSoundSourceGroup::set_group_name(const String& group_name)
	{
		m_groupName = group_name;
	}

	mint::String CSoundSourceGroup::get_group_name() const
	{
		return m_groupName;
	}

	mint::u64 CSoundSourceGroup::get_group_identifier() const
	{
		return mint::algorithm::djb_hash(m_groupName);
	}

	void CSoundSourceGroup::set_sound_group_settings(const SSoundSourceGroupSettings& settings)
	{
		m_settings = settings;
	}

	mint::u64 CSoundSourceGroup::get_parent_group() const
	{
		return m_parent;
	}

	mint::Vector< mint::u64 > CSoundSourceGroup::get_children() const
	{
		return m_children;
	}

	void CSoundSourceGroup::remove_child(u64 child_identifier)
	{
		mint::algorithm::vector_erase(m_children, child_identifier);
	}

	mint::sound::SSoundSourceGroupSettings CSoundSourceGroup::get_sound_group_settings() const
	{
		return m_settings;
	}

}