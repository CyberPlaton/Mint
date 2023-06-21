#if MINT_DISTR
#else
#include "TransformComponentEditor.h"

namespace mint::editor
{
	void CTransformComponentEditor::on_terminate()
	{

	}


	void CTransformComponentEditor::on_update(f32 dt)
	{

	}


	void CTransformComponentEditor::on_ui_frame()
	{
		MINT_PROFILE_SCOPE("CTransformComponentEditor::on_ui_frame", "Editor::ComponentEditor");

		auto w = GlobalData::Get().s_DefaultEditorDialogWidth;
		auto h = GlobalData::Get().s_DefaultEditorDialogHeight;

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ get_window_width() / 2.0f - w / 2.0f,  get_window_height() / 2.0f - h / 2.0f }, ImGuiCond_Once);

		ImGui::Begin("Transform", &m_active, get_flags());

		show_members();

		ImGui::End();
	}


	void CTransformComponentEditor::show_members()
	{
		auto entity = get_metaclass()->get_metaclass_entity();
		auto gd = GlobalData::Get();
		ImGuiID slid = 10000;
		ImGuiID scid = 20000;

		auto& transform = MINT_ACTIVE_SCENE()->get_registry()->get_component< mint::component::STransform >(entity);
		
		auto previous_position = transform.m_position;

		CUI::edit_field_vec2(transform.m_position, gd.s_EditorTransformMinPosition, gd.s_EditorTransformMaxPosition,
							 "Position", "Entities current position in the world, this affects all children positions recursively", slid++, scid++);
		
		ImGui::Separator();


		CUI::edit_field_vec2(transform.m_scale, gd.s_EditorTransformMinScale, gd.s_EditorTransformMaxScale,
							 "Scale", "Entities current scale, this affects all children scale values recursively", slid++, scid++, ImGuiSliderFlags_Logarithmic);

		ImGui::Separator();


		CUI::edit_field_f32(transform.m_rotation, gd.s_EditorTransformMinRotation, gd.s_EditorTransformMaxRotation,
							"Rotation", "Entities current rotation, this affects all children rotation values recursively", slid++, scid++, ImGuiSliderFlags_None, 0.05f);


		CUCA::transform_set_transform_matrix(entity, glm::translate(Mat4(1.0f), Vec3(transform.m_position, 0.0f)) *

													 glm::rotate(Mat4(1.0f), transform.m_rotation, Vec3(0.0f, 0.0f, 1.0f)) *

													 glm::scale(Mat4(1.0f), Vec3(transform.m_scale, 0.0f)));


		
		auto displacement = transform.m_position - previous_position;

		if (displacement.x >= b2_aabbExtension || displacement.x <= b2_aabbExtension ||
			displacement.y >= b2_aabbExtension || displacement.y <= b2_aabbExtension )
		{
			auto dest_rect = CUCA::sprite_get_destination_rect(entity);

			auto aabb = mint::algorithm::compute_aabb(dest_rect);

			CWorldQuery::Get().update_entity_proxy(entity, aabb, displacement);
		}
	}


	CTransformComponentEditor::CTransformComponentEditor(reflection::CMetaClass* metaclass) : CComponentEditor(metaclass)
	{
		set_ready(true);
		set_active(true);
	}


}
#endif