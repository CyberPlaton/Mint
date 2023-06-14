#if MINT_DISTR
#else
#include "MaterialEditor.h"

namespace mint::editor 
{

	CMaterialEditor::CMaterialEditor(entt::entity entity, mint::fx::CMaterial* material, u64 material_index) :
		m_ready(false), m_active(false), m_entity(entity), m_windowFlags(0), m_materialIndex(material_index)
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
		MINT_PROFILE_SCOPE("Editor::MaterialEditor", "CMaterialEditor::on_ui_frame");

		auto w = GlobalData::Get().s_DefaultEditorTextEditorWidth;
		auto h = GlobalData::Get().s_DefaultEditorTextEditorHeight;

		String text = String(ICON_FA_WAND_MAGIC_SPARKLES) + " Material Editor for \"" + CUCA::identifier_get_debug_name(m_entity) + "\"";

		ImGui::SetNextWindowSize({ w, h }, ImGuiCond_Once);
		ImGui::Begin(text.c_str(), &m_active, m_windowFlags);

		main_frame();
		
		ImGui::End();

		if (m_addingDynamicUniform) show_dialog_add_dynamic_uniform();
		if (m_addingStaticUniform) show_dialog_add_static_uniform();
		if (m_exportingMaterial) show_dialog_export_material();
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
		CUI::edit_field_uint64(m_materialIndex, 0, MINTFX_MATERIAL_COUNT_MAX, "Material Index", "", sid++, scid++);
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

		if (ImGui::Button(ICON_FA_PLUS " ##Add S"))
		{
			m_addingStaticUniform = true;
		}

		auto& suniforms = m_materialDefinition.m_staticUniforms.get_all();

		for (auto& it : suniforms)
		{
			show_uniform_edit(it, sid++, scid++);
		}


		ImGui::SeparatorText("Dynamic Shader Uniforms");

		if (ImGui::Button(ICON_FA_PLUS " ##Add D"))
		{
			m_addingDynamicUniform = true;
		}

		auto& duniforms = m_materialDefinition.m_dynamicUniforms.get_all();

		for (auto& it : duniforms)
		{
			show_uniform_edit(it, sid++, scid++);
		}


		if (ImGui::Button("Apply Material"))
		{
			CUCA::sprite_set_material_at_index(m_entity, m_materialDefinition, m_materialIndex);
		}
		if (ImGui::Button("Export Material"))
		{
			m_exportingMaterial = true;
		}
	}


	void CMaterialEditor::show_dialog_add_dynamic_uniform()
	{
		MINT_PROFILE_SCOPE("Editor::MaterialEditor", "CMaterialEditor::show_dialog_add_dynamic_uniform");

		ImGuiID sid = 30000;
		ImGuiID scid = 40000;

		ImGui::Begin(ICON_FA_PLUS " Dynamic Uniform", &m_addingDynamicUniform);

		CUI::edit_field_string(uname, "Name", "", sid++, scid++);

		if (ImGui::Combo("Uniform Type", &uniform_type, s_UniformType, IM_ARRAYSIZE(s_UniformType)))
		{
		}
		
		switch (uniform_type)
		{
		case 0:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

 				uniform.set(uname, (void*)&fvalue, SHADER_UNIFORM_FLOAT);

				m_materialDefinition.m_dynamicUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 1:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&v2value, SHADER_UNIFORM_VEC2);

				m_materialDefinition.m_dynamicUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 2:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&v3value, SHADER_UNIFORM_VEC3);

				m_materialDefinition.m_dynamicUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 3:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&v4value, SHADER_UNIFORM_VEC4);

				m_materialDefinition.m_dynamicUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 4:
		{ 		
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&ivalue, SHADER_UNIFORM_INT);

				m_materialDefinition.m_dynamicUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 5:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&svalue, SHADER_UNIFORM_SAMPLER2D);

				m_materialDefinition.m_dynamicUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		}

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_XMARK))
		{
			m_addingDynamicUniform = false;
		}

		ImGui::End();
	}

	void CMaterialEditor::show_dialog_add_static_uniform()
	{
		MINT_PROFILE_SCOPE("Editor::MaterialEditor", "CMaterialEditor::show_dialog_add_static_uniform");

		ImGuiID sid = 30000;
		ImGuiID scid = 40000;

		ImGui::Begin(ICON_FA_PLUS " Static Uniform", &m_addingStaticUniform);

		CUI::edit_field_string(uname, "Name", "", sid++, scid++);

		if (ImGui::Combo("Uniform Type", &uniform_type, s_UniformType, IM_ARRAYSIZE(s_UniformType)))
		{
		}

		switch (uniform_type)
		{
		case 0:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&fvalue, SHADER_UNIFORM_FLOAT);

				m_materialDefinition.m_staticUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 1:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&v2value, SHADER_UNIFORM_VEC2);

				m_materialDefinition.m_staticUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 2:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&v3value, SHADER_UNIFORM_VEC3);

				m_materialDefinition.m_staticUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 3:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&v4value, SHADER_UNIFORM_VEC4);

				m_materialDefinition.m_staticUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 4:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&ivalue, SHADER_UNIFORM_INT);

				m_materialDefinition.m_staticUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		case 5:
		{
			if (ImGui::SmallButton(ICON_FA_CHECK))
			{
				mint::fx::SShaderUniform uniform;

				uniform.set(uname, (void*)&svalue, SHADER_UNIFORM_SAMPLER2D);

				m_materialDefinition.m_staticUniforms.add(mint::algorithm::djb_hash(uname), uniform);
			}

			break;
		}
		}

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_XMARK))
		{
			m_addingStaticUniform = false;
		}

		ImGui::End();
	}


	void CMaterialEditor::show_uniform_edit(mint::fx::SShaderUniform& uniform, ImGuiID id, ImGuiID scalar_id)
	{
		MINT_PROFILE_SCOPE("Editor::MaterialEditor", "CMaterialEditor::show_uniform_edit");

		switch (uniform.get_type())
		{
		case SHADER_UNIFORM_FLOAT:
		{
			auto value = uniform.get_as< f32* >();

			CUI::edit_field_f32(*value, -GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);

			break;
		}
		case SHADER_UNIFORM_VEC2:
		{
			auto value = uniform.get_as< Vec2* >();

			CUI::edit_field_vec2(*value, -GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);

			break;
		}
		case SHADER_UNIFORM_VEC3:
		{
			auto value = uniform.get_as< Vec3* >();

			CUI::edit_field_vec3(*value, -GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);

			break;
		}
		case SHADER_UNIFORM_VEC4:
		{
			auto value = uniform.get_as< Vec4* >();

			CUI::edit_field_vec4(*value, -GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);

			break;
		}
		case SHADER_UNIFORM_INT:
		{
			auto value = uniform.get_as< s64* >();

			CUI::edit_field_sint64(*value, -GlobalData::Get().s_EditorTransformMinPosition, GlobalData::Get().s_EditorTransformMaxPosition, uniform.get_name(), "", id, scalar_id);

			break;
		}
		case SHADER_UNIFORM_SAMPLER2D:
		{
			auto value = uniform.get_as< String* >();

			CUI::edit_field_string(*value, uniform.get_name(), "", id, scalar_id);

			break;
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

	void CMaterialEditor::show_dialog_export_material()
	{
		MINT_PROFILE_SCOPE("Editor::MaterialEditor", "CMaterialEditor::show_dialog_export_material");

		static String path;
		static String name;

		ImGui::Begin(ICON_FA_CODE " Material Export", &m_exportingMaterial);

		CUI::edit_field_string(path, "Path", "", 10000, 20000);

		CUI::edit_field_string(name, "Filename", "", 10001, 20001);

		if (ImGui::SmallButton(ICON_FA_CHECK))
		{
			auto scene_path = MINT_ACTIVE_SCENE()->get_scene_full_path();

			CFilesystem fs(scene_path);

			if (fs.forward("assets") &&  fs.forward(path))
			{
				if (CFilesystem::create_file(fs.get_current_directory(), name, String("mat")))
				{
					// Set Filesystem to be at the created File.
					CFilesystem assetfs(fs);

					fs.forward(name + ".mat");


					// Create document for exporting.
					maml::CDocument document;

					auto material = document.create_node("material");

					CSerializer::export_string(m_materialDefinition.m_materialName, "name", material);
					CSerializer::export_string(m_materialDefinition.m_textureName, "texture", material);
					CSerializer::export_string(m_materialDefinition.m_shaderProgramName, "shader", material);
					CSerializer::export_uint((u64)m_materialDefinition.m_blendMode, "blendingmode", material);
					CSerializer::export_uint((u64)m_materialDefinition.m_srcBlendFactor, "blendingsrcfactor", material);
					CSerializer::export_uint((u64)m_materialDefinition.m_dstBlendFactor, "blendingdstfactor", material);
					CSerializer::export_uint((u64)m_materialDefinition.m_blendingEquation, "blendingequation", material);

					auto staticuniforms = document.create_node("staticuniforms", material);
					auto dynamicuniforms = document.create_node("dynamicuniforms", material);

					for (auto& su : m_materialDefinition.m_staticUniforms.get_all())
					{
						export_uniform(su, staticuniforms);
					}

					for (auto& du : m_materialDefinition.m_dynamicUniforms.get_all())
					{
						export_uniform(du, dynamicuniforms);
					}

					if (document.save_document(fs.get_current_directory().as_string()))
					{
						// Create Material asset file.
						if (CFilesystem::create_file(assetfs.get_current_directory(), name, String("material")) &&
							assetfs.forward(name + ".material"))
						{
							String data;
							data += "asset:\n";
							data += "\t" + String("name=\"") + name + "\"\n";
							data += "\t" + String("type=\"") + ".material" + "\"\n";
							data += "\t" + String("source=\"") + name + ".mat" + "\"\n";
							data += "\t" + String("author=\"") + "\"\n";
							data += "\t" + String("version=") + std::to_string(1) + "\n";
							data += "\t" + String("description=\"") + "\"\n";
							data += "end\n";
							data += "\0";

							CFileReaderWriter::write_to_file_at_path(assetfs.get_current_directory().as_string(), (void*)data.c_str(), data.size());
						}
					}
					else
					{
						MINT_LOG_ERROR("[{:.4f}][CMaterialEditor::show_dialog_export_material] Failed exporting Material \"{}\" to \"{}\"!", MINT_APP_TIME, name, fs.get_current_directory().as_string());
					}

					m_exportingMaterial = false;
				}
			}
		}

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_XMARK))
		{
			m_exportingMaterial = false;
		}

		ImGui::End();
	}

	void CMaterialEditor::export_uniform(mint::fx::SShaderUniform& uniform, maml::SNode* node)
	{
		MINT_PROFILE_SCOPE("Editor::MaterialEditor", "CMaterialEditor::export_uniform");

		switch (uniform.get_type())
		{
		case SHADER_UNIFORM_FLOAT:
		{
			auto v = *uniform.get_as< f32* >();

			CSerializer::export_float(v, uniform.get_name(), node);
			break;
		}
		case SHADER_UNIFORM_VEC2:
		{
			auto v = *uniform.get_as< Vec2* >();

			CSerializer::export_vec2(v, uniform.get_name(), node);
			break;
		}
		case SHADER_UNIFORM_VEC3:
		{
			auto v = *uniform.get_as< Vec3* >();

			CSerializer::export_vec3(v, uniform.get_name(), node);
			break;
		}
		case SHADER_UNIFORM_VEC4:
		{
			auto v = *uniform.get_as< Vec4* >();

			CSerializer::export_vec4(v, uniform.get_name(), node);
			break;
		}
		case SHADER_UNIFORM_INT:
		{
			auto v = *uniform.get_as< s64* >();

			CSerializer::export_sint(v, uniform.get_name(), node);
			break;
		}
		case SHADER_UNIFORM_SAMPLER2D:
		{
			auto v = *uniform.get_as< String* >();

			CSerializer::export_string(v, uniform.get_name(), node);
			break;
		}
		default: { return; }
		}
	}

}

#endif