#if MINT_DISTR
#else
#include "SpriteComponentEditor.h"

namespace mint::editor
{
	void CSpriteComponentEditor::on_terminate()
	{

	}


	void CSpriteComponentEditor::on_update(f32 dt)
	{

	}


	void CSpriteComponentEditor::on_ui_frame()
	{
		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("Sprite", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CSpriteComponentEditor::show_members()
	{
		auto entity = get_metaclass()->get_metaclass_entity();
		ImGuiID slid = 10000;
		ImGuiID scid = 20000;

		auto& materials = CUCA::sprite_get_all_materials(entity);

		auto main_material = CUCA::sprite_get_main_material(entity);

		if(ImGui::CollapsingHeader("Main Material"))
		{
			ImGui::Text(TextFormat("Texture Handle: %zu", main_material->get_texture_handle()));
			ImGui::Text(TextFormat("Texture Size: {%.3f:%.3f}", main_material->get_texture_dimension().x, main_material->get_texture_dimension().y));
		}
		
		ImGui::Separator();

		auto visible = CUCA::sprite_is_visible(entity);
		auto internal_visible = CUCA::sprite_is_internal_visible(entity);
		auto depth = CUCA::sprite_get_depth(entity);
		auto flipx = CUCA::sprite_is_flipped_x(entity);
		auto flipy = CUCA::sprite_is_flipped_y(entity);
		auto origin = CUCA::sprite_get_origin(entity);
		auto color = CUCA::sprite_get_color(entity);
		auto rect = CUCA::sprite_get_source_rect(entity);
		

		CUI::edit_field_bool(visible, "Visible", "", slid++, scid++);
		CUI::edit_field_bool(internal_visible, "Internal Visible", "", slid++, scid++);
		CUI::edit_field_bool(flipx, "Flip Horizontally", "", slid++, scid++);
		CUI::edit_field_bool(flipy, "Flip Vertically", "", slid++, scid++);
		CUI::edit_field_uint64(depth, 0, MINT_SAS_RENDERING_LAYERS_MAX, "Layer", "", slid++, scid++);
		CUI::edit_field_vec2(origin, 0.0f, 100.0f, "Sprite Origin", "", slid++, scid++);
		CUI::edit_field_rect(rect, 0.0f, 1.0f, "Source Rectangle", "", slid++, scid++, ImGuiSliderFlags_Logarithmic, 0.01f);
		CUI::edit_field_color(color, 0, 255, "Sprite Tint", "", slid++, scid++, ImGuiSliderFlags_Logarithmic);


		CUCA::sprite_set_is_visible(entity, visible);
		CUCA::sprite_set_depth(entity, depth);
		CUCA::sprite_set_is_flipped_x(entity, flipx);
		CUCA::sprite_set_is_flipped_y(entity, flipy);
		CUCA::sprite_set_origin(entity, origin);
		CUCA::sprite_set_color(entity, color);
		CUCA::sprite_set_source_rect(entity, rect);
	}


	CSpriteComponentEditor::CSpriteComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


	void CSpriteComponentEditor::show_materials(const Vector< mint::fx::CMaterial >& materials)
	{
		ImGui::BeginChild("Entity Materials");

		for (const auto& material: materials)
		{
			show_material(material);
		}

		ImGui::End();
	}


	void CSpriteComponentEditor::show_material(const mint::fx::CMaterial& material)
	{
	}


}
#endif