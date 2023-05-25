#if MINT_DISTR
#else
#ifndef _MINTEDITOR_MATERIAL_EDITOR_H_
#define _MINTEDITOR_MATERIAL_EDITOR_H_


#include "../Common/EditorCommon.h"
#include "Graphics/Shaders/MaterialManager.h"


namespace mint::editor
{

	class CMaterialEditor
	{
	public:
		CMaterialEditor(entt::entity entity);
		~CMaterialEditor();


		void on_ui_frame();

		bool is_ready();

		bool is_active();


	private:
		ImGuiWindowFlags m_windowFlags;

		entt::entity m_entity;

		bool m_active;

		bool m_ready;


	};
}


#endif
#endif