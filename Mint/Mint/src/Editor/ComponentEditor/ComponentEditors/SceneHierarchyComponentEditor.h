#if MINT_DISTR
#else
#ifndef _MINTEDITOR_SCENE_HIERARCHY_COMPONENT_EDITOR_H_
#define _MINTEDITOR_SCENE_HIERARCHY_COMPONENT_EDITOR_H_


#include "../Common/ComponentEditor.h"


namespace mint::editor
{

	class CSceneHierarchyComponentEditor : public CComponentEditor
	{
	public:
		CSceneHierarchyComponentEditor(reflection::CMetaClass* metaclass);

		void on_terminate() override final;

		void on_update(f32 dt) override final;

		void on_ui_frame() override final;

	private:
		void show_members();

		void show_child_options(entt::entity parent, entt::entity child);

		void show_parent_options(entt::entity parent, entt::entity child);
	};

}

#endif
#endif