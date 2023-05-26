#if MINT_DISTR
#else
#ifndef _MINTEDITOR_MATERIAL_EDITOR_H_
#define _MINTEDITOR_MATERIAL_EDITOR_H_


#include "../Common/EditorCommon.h"
#include "Graphics/Shaders/MaterialManager.h"
#include "RessourceManagement/TextureManager.h"


namespace mint::editor
{

	class CMaterialEditor
	{
	public:
		CMaterialEditor(entt::entity entity, mint::fx::CMaterial* material);
		~CMaterialEditor();


		void on_update(f32 dt);

		void on_ui_frame();

		bool is_ready();

		bool is_active();


	private:
		ImGuiWindowFlags m_windowFlags;

		mint::fx::CMaterial* m_material;

		entt::entity m_entity;

		bool m_active;

		bool m_ready;


	private:
		void main_frame();

		void show_material(mint::fx::CMaterial* material, u32 material_index);
	};
}


#endif
#endif