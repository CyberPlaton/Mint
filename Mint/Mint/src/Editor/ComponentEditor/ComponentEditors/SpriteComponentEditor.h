#if MINT_DISTR
#else
#ifndef _MINTEDITOR_SPRITE_COMPONENT_EDITOR_H_
#define _MINTEDITOR_SPRITE_COMPONENT_EDITOR_H_


#include "../Common/ComponentEditor.h"
#include "Scripting/BehaviorEngine.h"


namespace mint::editor
{

	class CSpriteComponentEditor : public CComponentEditor
	{
	public:
		CSpriteComponentEditor(reflection::CMetaClass* metaclass);

		void on_terminate() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

	private:
		void show_members();

		void show_materials(const Vector< mint::fx::CMaterial >& materials);

		void show_material(const mint::fx::CMaterial& material);
	};

}

#endif
#endif