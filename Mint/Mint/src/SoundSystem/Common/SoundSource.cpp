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
		m_group(nullptr)
	{

	}

	CSoundSource::~CSoundSource()
	{
		m_group = nullptr;
	}

}
