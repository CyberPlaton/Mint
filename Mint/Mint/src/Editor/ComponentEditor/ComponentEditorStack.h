#if MINT_DISTR
#else
#ifndef _MINTEDITOR_COMPONENT_EDITOR_STACK_H_
#define _MINTEDITOR_COMPONENT_EDITOR_STACK_H_


#include "ComponentEditors/AnimatedSpriteComponentEditor.h"
#include "ComponentEditors/GeneralComponentEditor.h"
#include "ComponentEditors/IdentifierComponentEditor.h"
#include "ComponentEditors/RigidBodyComponentEditor.h"
#include "ComponentEditors/SceneHierarchyComponentEditor.h"
#include "ComponentEditors/ScriptComponentEditor.h"
#include "ComponentEditors/SpriteComponentEditor.h"
#include "ComponentEditors/TransformComponentEditor.h"


namespace mint::editor
{

	class CComponentEditorStack
	{
	public:
		CComponentEditorStack() = default;
		~CComponentEditorStack();


		void on_ui_frame();

		void on_update(f32 dt);


		bool try_push_component_editor(reflection::CMetaClass* metaclass);

		void pop_component_editor();

		void remove_component_editor(const String& component_name);

		void clear_all_component_editors();

		void print_registered_component_editors();

		s64 find_component_editor(const String& component_name);


	private:
		Vector< IComponentEditor* > m_componentEditors;

	};

}

#endif
#endif