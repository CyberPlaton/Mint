#if MINT_DISTR
#else
#include "MaterialEditor.h"

namespace mint::editor 
{

	CMaterialEditor::CMaterialEditor(entt::entity entity, mint::fx::CMaterial* material) :
		m_ready(false), m_active(false), m_entity(entity), m_windowFlags(0)
	{
		if (is_handle_valid(entity_get_handle(entity)) && material != nullptr)
		{
			// Copy material data.
			m_materialDefinition.m_materialName = material->m_materialName;
			m_materialDefinition.m_blendMode = material->m_blendMode;
			m_materialDefinition.m_srcBlendFactor = material->m_srcBlendFactor;
			m_materialDefinition.m_dstBlendFactor = material->m_dstBlendFactor;
			m_materialDefinition.m_blendingEquation = material->m_blendingEquation;
			m_materialDefinition.m_staticUniforms = material->m_staticUniforms;
			m_materialDefinition.m_dynamicUniforms = material->m_dynamicUniforms;
			m_materialDefinition.m_textureName = material->m_textureName;
			m_materialDefinition.m_shaderProgramName = material->m_shaderProgramName;


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

		String text = String(ICON_FA_WAND_MAGIC_SPARKLES) + " Material Editor for Material \"" + m_materialDefinition.m_materialName + "\" of \"" + CUCA::identifier_get_debug_name(m_entity) + "\"";

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
		ImGuiID sid = 10000;
		ImGuiID scid = 20000;

		CUI::edit_field_string(m_materialDefinition.m_materialName, "Material Name", "", sid++, scid++);
		CUI::edit_field_string(m_materialDefinition.m_textureName, "Texture Name", "", sid++, scid++);
		CUI::edit_field_string(m_materialDefinition.m_shaderProgramName, "Shader Program Name", "", sid++, scid++);

		ImGui::SeparatorText("Blending");

		int current_blending_mode = (int)m_materialDefinition.m_blendMode;
		if (ImGui::Combo("Mode", &current_blending_mode, s_BlendingModeOptions, IM_ARRAYSIZE(s_BlendingModeOptions)))
		{
			m_materialDefinition.m_blendMode = (BlendMode)current_blending_mode;
		}

		int current_blending_src = get_blending_src_factor_option_number(m_materialDefinition.m_srcBlendFactor);
		if (ImGui::Combo("Source Factor", &current_blending_src, s_BlendingFactorOptions, IM_ARRAYSIZE(s_BlendingFactorOptions)))
		{
			set_blending_src_factor(current_blending_src);
		}

		int current_blending_dst = get_blending_dst_factor_option_number(m_materialDefinition.m_dstBlendFactor);
		if (ImGui::Combo("Destination Factor", &current_blending_dst, s_BlendingFactorOptions, IM_ARRAYSIZE(s_BlendingFactorOptions)))
		{
			set_blending_dst_factor(current_blending_dst);
		}

		int current_blending_equat = get_blending_equation_option_number(m_materialDefinition.m_blendingEquation);
		if (ImGui::Combo("Equation", &current_blending_equat, s_BlendingEquationOptions, IM_ARRAYSIZE(s_BlendingEquationOptions)))
		{
			set_blending_equation(current_blending_equat);
		}


		ImGui::SeparatorText("Static Shader Uniforms");

		for (auto& it : m_materialDefinition.m_staticUniforms)
		{
			show_uniform_edit(it, sid++, scid++);
		}


		ImGui::SeparatorText("Dynamic Shader Uniforms");

		for (auto& it : m_materialDefinition.m_dynamicUniforms)
		{
			show_uniform_edit(it, sid++, scid++);
		}
	}


	void CMaterialEditor::show_uniform_edit(mint::fx::SShaderUniform& uniform, ImGuiID id, ImGuiID scalar_id)
	{
		switch (uniform.get_type())
		{
		case SHADER_UNIFORM_FLOAT:
		{
			auto value = uniform.get< f32 >();

			CUI::edit_field_f32(*value, GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);
		}
		case SHADER_UNIFORM_VEC2:
		{
			auto value = uniform.get< Vec2 >();

			CUI::edit_field_vec2(*value, GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);
		}
		case SHADER_UNIFORM_VEC3:
		{
			auto value = uniform.get< Vec3 >();

			CUI::edit_field_vec3(*value, GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);
		}
		case SHADER_UNIFORM_VEC4:
		{
			auto value = uniform.get< Vec4 >();

			CUI::edit_field_vec4(*value, GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);
		}
		case SHADER_UNIFORM_INT:
		{
			auto value = uniform.get< s32 >();

			CUI::edit_field_sint32(*value, GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);
		}
		case SHADER_UNIFORM_SAMPLER2D:
		{
			auto value = uniform.get< String >();

			CUI::edit_field_string(*value, uniform.get_name(), "", id, scalar_id);
		}
		default: { return; }
		}
	}


	void CMaterialEditor::on_update(f32 dt)
	{
	}


	void CMaterialEditor::set_blending_src_factor(int option_number)
	{
		switch (option_number)
		{
		case 0: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_Zero; break;
		case 1: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_One; break;
		case 2: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_SrcColor; break;
		case 3: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_OneMinusSrcColor; break;
		case 4: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_SrcAlpha; break;
		case 5: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_OneMinusSrcAlpha; break;
		case 6: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_DstAlpha; break;
		case 7: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_DstColor; break;
		case 8: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_OneMinusDstAlpha; break;
		case 9: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_SrcAlphaSaturate; break;
		case 10: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_ConstantColor; break;
		case 11: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_OneMinusConstantColor; break;
		case 12: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_ConstantAlpha; break;
		case 13: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_OneMinusConstantAlpha; break;
		default: m_materialDefinition.m_srcBlendFactor = mint::fx::BlendingFactor_Zero; break;
		}
	}

	void CMaterialEditor::set_blending_dst_factor(int option_number)
	{
		switch (option_number)
		{
		case 0: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_Zero; break;
		case 1: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_One; break;
		case 2: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_SrcColor; break;
		case 3: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_OneMinusSrcColor; break;
		case 4: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_SrcAlpha; break;
		case 5: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_OneMinusSrcAlpha; break;
		case 6: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_DstAlpha; break;
		case 7: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_DstColor; break;
		case 8: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_OneMinusDstAlpha; break;
		case 9: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_SrcAlphaSaturate; break;
		case 10: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_ConstantColor; break;
		case 11: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_OneMinusConstantColor; break;
		case 12: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_ConstantAlpha; break;
		case 13: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_OneMinusConstantAlpha; break;
		default: m_materialDefinition.m_dstBlendFactor = mint::fx::BlendingFactor_Zero; break;
		}
	}

	void CMaterialEditor::set_blending_equation(int option_number)
	{
		switch (option_number)
		{
		case 0: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_Add; break;
		case 1: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_Min; break;
		case 2: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_Max; break;
		case 3: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_Subtract; break;
		case 4: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_ReverseSubtract; break;
		case 5: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_BlendEquationRGB; break;
		case 6: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_BlendEquationAlpha; break;
		case 7: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_BlendDstRGB; break;
		case 8: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_BlendSrcRGB; break;
		case 9: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_BlendDstAlpha; break;
		case 10: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_BlendSrcAlpha; break;
		case 11: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_BlendColor; break;
		default: m_materialDefinition.m_blendingEquation = mint::fx::BlendingEquation_BlendColor; break;
		}
	}

	int CMaterialEditor::get_blending_src_factor_option_number(uint32_t blending_factor)
	{
		switch ((mint::fx::BlendingFactor)blending_factor)
		{
		case mint::fx::BlendingFactor_Zero:						return 0;
		case mint::fx::BlendingFactor_One:						return 1;
		case mint::fx::BlendingFactor_SrcColor:					return 2;
		case mint::fx::BlendingFactor_OneMinusSrcColor:			return 3;
		case mint::fx::BlendingFactor_SrcAlpha:					return 4;
		case mint::fx::BlendingFactor_OneMinusSrcAlpha:			return 5;
		case mint::fx::BlendingFactor_DstAlpha:					return 6;
		case mint::fx::BlendingFactor_DstColor:					return 7;
		case mint::fx::BlendingFactor_OneMinusDstAlpha:			return 8;
		case mint::fx::BlendingFactor_SrcAlphaSaturate:			return 9;
		case  mint::fx::BlendingFactor_ConstantColor:			return 10;
		case  mint::fx::BlendingFactor_OneMinusConstantColor:	return 11;
		case  mint::fx::BlendingFactor_ConstantAlpha:			return 12;
		case  mint::fx::BlendingFactor_OneMinusConstantAlpha:	return 13;
		default: return 0;
		}
	}

	int CMaterialEditor::get_blending_dst_factor_option_number(uint32_t blending_factor)
	{
		switch ((mint::fx::BlendingFactor)blending_factor)
		{
		case mint::fx::BlendingFactor_Zero:						return 0;
		case mint::fx::BlendingFactor_One:						return 1;
		case mint::fx::BlendingFactor_SrcColor:					return 2;
		case mint::fx::BlendingFactor_OneMinusSrcColor:			return 3;
		case mint::fx::BlendingFactor_SrcAlpha:					return 4;
		case mint::fx::BlendingFactor_OneMinusSrcAlpha:			return 5;
		case mint::fx::BlendingFactor_DstAlpha:					return 6;
		case mint::fx::BlendingFactor_DstColor:					return 7;
		case mint::fx::BlendingFactor_OneMinusDstAlpha:			return 8;
		case mint::fx::BlendingFactor_SrcAlphaSaturate:			return 9;
		case  mint::fx::BlendingFactor_ConstantColor:			return 10;
		case  mint::fx::BlendingFactor_OneMinusConstantColor:	return 11;
		case  mint::fx::BlendingFactor_ConstantAlpha:			return 12;
		case  mint::fx::BlendingFactor_OneMinusConstantAlpha:	return 13;
		default: return 0;
		}
	}

	int CMaterialEditor::get_blending_equation_option_number(uint32_t blending_equation)
	{
		switch ((mint::fx::BlendingEquation)blending_equation)
		{
		case mint::fx::BlendingEquation_Add:				return 0;
		case mint::fx::BlendingEquation_Min:				return 1;
		case mint::fx::BlendingEquation_Max:				return 2;
		case mint::fx::BlendingEquation_Subtract:			return 3;
		case mint::fx::BlendingEquation_ReverseSubtract:	return 4;
		case mint::fx::BlendingEquation_BlendEquationRGB:	return 5;
		case mint::fx::BlendingEquation_BlendEquationAlpha:	return 6;
		case mint::fx::BlendingEquation_BlendDstRGB:		return 7;
		case mint::fx::BlendingEquation_BlendSrcRGB:		return 8;
		case mint::fx::BlendingEquation_BlendDstAlpha:		return 9;
		case  mint::fx::BlendingEquation_BlendSrcAlpha:		return 10;
		case  mint::fx::BlendingEquation_BlendColor:		return 11;
		default: return 0;
		}
	}

}

#endif