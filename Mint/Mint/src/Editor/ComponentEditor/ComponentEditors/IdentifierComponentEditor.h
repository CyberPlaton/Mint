#if MINT_DISTR
#else
#ifndef _MINTEDITOR_IDENTIFIER_COMPONENT_EDITOR_H_
#define _MINTEDITOR_IDENTIFIER_COMPONENT_EDITOR_H_


#include "../Common/ComponentEditor.h"


namespace mint::editor
{

	class CIdentifierComponentEditor : public CComponentEditor
	{
	public:
		CIdentifierComponentEditor(reflection::CMetaClass* metaclass);

		void on_terminate() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

	private:
		void show_members();
	};

}

#endif
#endif