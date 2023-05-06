#if MINT_DISTR
#else
#ifndef _MINTEDITOR_TRANSFORM_COMPONENT_EDITOR_H_
#define _MINTEDITOR_TRANSFORM_COMPONENT_EDITOR_H_


#include "../Common/ComponentEditor.h"


namespace mint::editor
{

	class CTransformComponentEditor : public CComponentEditor
	{
	public:
		CTransformComponentEditor(reflection::CMetaClass* metaclass);

		void on_terminate() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

	private:
		void show_members();
	};

}

#endif
#endif