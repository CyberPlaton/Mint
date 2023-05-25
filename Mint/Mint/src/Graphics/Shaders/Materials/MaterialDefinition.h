#ifndef _MINTFX_MATERIAL_DEFINITION_H_
#define _MINTFX_MATERIAL_DEFINITION_H_


#include "Common/Common.h"
#include "Utility/STL/Map.h"
#include "ShaderUniform.h"
#include "Common/Algorithm.h"


namespace mint::fx
{

	enum BlendingFactor
	{
		BlendingFactor_Zero = 0,
		BlendingFactor_One = 1,
		BlendingFactor_SrcColor = RL_SRC_COLOR,
		BlendingFactor_OneMinusSrcColor = RL_ONE_MINUS_SRC_COLOR,
		BlendingFactor_SrcAlpha = RL_SRC_ALPHA,
		BlendingFactor_OneMinusSrcAlpha = RL_ONE_MINUS_SRC_ALPHA,
		BlendingFactor_DstAlpha = RL_DST_ALPHA,
		BlendingFactor_DstColor = RL_DST_COLOR,
		BlendingFactor_OneMinusDstAlpha = RL_ONE_MINUS_DST_ALPHA,
		BlendingFactor_SrcAlphaSaturate = RL_SRC_ALPHA_SATURATE,
		BlendingFactor_ConstantColor = RL_CONSTANT_COLOR,
		BlendingFactor_OneMinusConstantColor = RL_ONE_MINUS_CONSTANT_COLOR,
		BlendingFactor_ConstantAlpha = RL_CONSTANT_ALPHA,
		BlendingFactor_OneMinusConstantAlpha = RL_ONE_MINUS_CONSTANT_ALPHA,

		BlendingFactor_Count = BlendingFactor_OneMinusConstantAlpha
	};

	enum BlendingEquation
	{
		BlendingEquation_Add = RL_FUNC_ADD,
		BlendingEquation_Min = RL_MIN,
		BlendingEquation_Max = RL_MAX,
		BlendingEquation_Subtract = RL_FUNC_SUBTRACT,
		BlendingEquation_ReverseSubtract = RL_FUNC_REVERSE_SUBTRACT,
		BlendingEquation_BlendEquationRGB = RL_BLEND_EQUATION_RGB,
		BlendingEquation_BlendEquationAlpha = RL_BLEND_EQUATION_ALPHA,
		BlendingEquation_BlendDstRGB = RL_BLEND_DST_RGB,
		BlendingEquation_BlendSrcRGB = RL_BLEND_SRC_RGB,
		BlendingEquation_BlendDstAlpha = RL_BLEND_DST_ALPHA,
		BlendingEquation_BlendSrcAlpha = RL_BLEND_SRC_ALPHA,
		BlendingEquation_BlendColor = RL_BLEND_COLOR,

		BlendingEquation_Count = BlendingEquation_BlendColor
	};

	struct SMaterialDefinition
	{
		String m_textureName;
		String m_shaderProgramName;

		BlendMode m_blendMode = BLEND_ALPHA;
		uint32_t m_srcBlendFactor = RL_SRC_ALPHA;
		uint32_t m_dstBlendFactor = RL_ONE_MINUS_SRC_ALPHA;
		uint32_t m_blendingEquation = RL_MAX;

		CMap< SShaderUniform > m_staticUniforms;
		CMap< SShaderUniform > m_dynamicUniforms;
	};
}


#endif