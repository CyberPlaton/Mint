#if MINT_DISTR
#else
#include "MaterialEditor.h"

namespace mint::editor 
{

	CMaterialEditor::CMaterialEditor(entt::entity entity, mint::fx::CMaterial* material) :
		m_ready(false), m_active(false), m_entity(entity), m_windowFlags(0), m_material(material)
	{
		if (is_handle_valid(entity_get_handle(entity)))
		{
			m_windowFlags = ImGuiWindowFlags_None;
			m_active = true;
			m_ready = true;
		}
	}

	CMaterialEditor::~CMaterialEditor()
	{

	}

	void CMaterialEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorTextEditorWidth;
		auto h = GlobalData::Get().s_DefaultEditorTextEditorHeight;

		String text = String(ICON_FA_WAND_MAGIC_SPARKLES) + " Material Editor for Material \"" + std::to_string(m_material->m_handle) + "\" of \"" + CUCA::identifier_get_debug_name(m_entity) + "\"";

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::Begin(text.c_str(), &m_active, m_windowFlags);

		main_frame();
		
		ImGui::End();
	}

	bool CMaterialEditor::is_ready()
	{
		return m_ready;
	}

	bool CMaterialEditor::is_active()
	{
		return m_active;
	}

	void CMaterialEditor::main_frame()
	{
		
	}

	void CMaterialEditor::on_update(f32 dt)
	{
		if (m_material == nullptr) { m_ready = false; m_active = false; }
	}

}

#endif