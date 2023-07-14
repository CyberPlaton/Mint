#if MINT_DISTR
#else
#ifndef _MINTEDITOR_SOUND_SOURCE_COMPONENT_EDITOR_H_
#define _MINTEDITOR_SOUND_SOURCE_COMPONENT_EDITOR_H_


#include "../Common/ComponentEditor.h"
#include "SoundSystem/SoundSystem.h"


namespace mint::editor
{

	class CSoundSourceComponentEditor : public CComponentEditor
	{
	public:
		CSoundSourceComponentEditor(reflection::CMetaClass* metaclass);

		void on_terminate() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

	private:
		void show_members();
	};

}

#endif
#endif