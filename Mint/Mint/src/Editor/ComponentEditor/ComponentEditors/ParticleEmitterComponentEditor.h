#if MINT_DISTR
#else
#ifndef _MINTEDITOR_PARTICLE_EMITTER_COMPONENT_EDITOR_H_
#define _MINTEDITOR_PARTICLE_EMITTER_COMPONENT_EDITOR_H_


#include "../Common/ComponentEditor.h"
#include "Graphics/ParticleSystem.h"


namespace mint::editor
{

	class CParticleEmitterComponentEditor : public CComponentEditor
	{
	public:
		CParticleEmitterComponentEditor(reflection::CMetaClass* metaclass);

		void on_terminate() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;


	private:
		bool m_exportingEmitter = false;

		mint::fx::SParticleDefinition m_emitter;


	private:
		void show_members();

		void show_dialog_export_emitter();
	};

}

#endif
#endif