#ifndef _MINTSOUND_SOUND_SOURCE_GROUP_H_
#define _MINTSOUND_SOUND_SOURCE_GROUP_H_


#include "SoundSource.h"
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

		virtual void set_group_sound_attributes(SSoundSourceSettings*) {};
	
	

	protected:

	};
}

#endif