#if MINT_DISTR
#else
#ifndef _MINTEDITOR_MATERIAL_EDITOR_H_
#define _MINTEDITOR_MATERIAL_EDITOR_H_


#include "../Common/EditorCommon.h"
#include "Graphics/Shaders/MaterialManager.h"
#include "RessourceManagement/TextureManager.h"


namespace mint::editor
{
	static const char* s_BlendingModeOptions[] = {
		STRING(BLEND_ALPHA),
		STRING(BLEND_ADDITIVE),
		STRING(BLEND_MULTIPLIED),
		STRING(BLEND_ADD_COLORS),
		STRING(BLEND_SUBTRACT_COLORS),
		STRING(BLEND_ALPHA_PREMULTIPLY)
	};

	static const char* s_BlendingFactorOptions[] = {
		STRING(BlendingFactor_Zero),
		STRING(BlendingFactor_One),
		STRING(BlendingFactor_SrcColor),
		STRING(BlendingFactor_OneMinusSrcColor),
		STRING(BlendingFactor_SrcAlpha),
		STRING(BlendingFactor_OneMinusSrcAlpha),
		STRING(BlendingFactor_DstAlpha),
		STRING(BlendingFactor_DstColor),
		STRING(BlendingFactor_OneMinusDstAlpha),
		STRING(BlendingFactor_SrcAlphaSaturate),
		STRING(BlendingFactor_ConstantColor),
		STRING(BlendingFactor_OneMinusConstantColor),
		STRING(BlendingFactor_ConstantAlpha),
		STRING(BlendingFactor_OneMinusConstantAlpha),
	};

	static const char* s_BlendingEquationOptions[] = {
		STRING(BlendingEquation_Add),
		STRING(BlendingEquation_Min),
		STRING(BlendingEquation_Max),
		STRING(BlendingEquation_Subtract),
		STRING(BlendingEquation_ReverseSubtract),
		STRING(BlendingEquation_BlendEquationRGB),
		STRING(BlendingEquation_BlendEquationAlpha),
		STRING(BlendingEquation_BlendDstRGB),
		STRING(BlendingEquation_BlendSrcRGB),
		STRING(BlendingEquation_BlendDstAlpha),
		STRING(BlendingEquation_BlendSrcAlpha),
		STRING(BlendingEquation_BlendColor)
	};


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

		mint::fx::SMaterialDefinition m_materialDefinition;

		entt::entity m_entity;

		bool m_active;

		bool m_ready;


	private:
		void main_frame();

		void set_blending_src_factor(int option_number);

		void set_blending_dst_factor(int option_number);

		void set_blending_equation(int option_number);

		int get_blending_src_factor_option_number(uint32_t blending_factor);

		int get_blending_dst_factor_option_number(uint32_t blending_factor);

		int get_blending_equation_option_number(uint32_t blending_equation);

		void show_uniform_edit(mint::fx::SShaderUniform& uniform, ImGuiID id, ImGuiID scalar_id);
	};
}


#endif
#endif