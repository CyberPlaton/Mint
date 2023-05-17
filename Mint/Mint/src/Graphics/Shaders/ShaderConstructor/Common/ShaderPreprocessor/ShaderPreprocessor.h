#ifndef _MINTFX_SHADER_PREPROCESSOR_H_
#define _MINTFX_SHADER_PREPROCESSOR_H_


#include "Common/Common.h"
#include "Utility/FileSystem/Filesystem.h"
#include "Utility/Logging/Logging.h"
#include "Utility/STL/Map.h"
#include "../GlobalShaderSettings.h"
#include "Common/Any.h"
#include "Common/Algorithm.h"


namespace mint::fx::sc
{
	enum PreprocessorOpCode
	{
		PreprocessorOpCode_Invalid = -1,
		PreprocessorOpCode_Include = 0,
		PreprocessorOpCode_Ifdef,
		PreprocessorOpCode_Else,
		PreprocessorOpCode_Endif,
	};

	enum PreprocessorValueType
	{
		PreprocessorValueType_Invalid = -1,
		PreprocessorValueType_String = 0,
		PreprocessorValueType_Identifier,
	};


	struct SPreprocessorValue
	{
		CAny m_value;
		PreprocessorValueType m_type;
	};


	class CShaderPreprocessor
	{
	public:
		String parse_shader_file(const String& file_path);


	private:
		String m_finalSource;

		String m_shaderName;

		String m_shaderPath;


		u64 m_currentLineNumber = 0;

		String m_currentLine;

	private:
		
		String _parse_shader_code_part(String& shader_code, u64 line_start = 0, u64 line_end = UINT_MAX);

		bool _get_next_line(std::istringstream& stream, String& output_string);

		bool _does_line_have_preprocessor_directive(const String& text);


		PreprocessorOpCode _get_op_code(const String& text);
		SPreprocessorValue _get_preprocessor_value(const String& text);

		bool _is_directive_opengl_native(const String& text);


		bool _is_ifdef(const String& text);

		bool _is_else(const String& text);

		bool _is_endif(const String& text);

		bool _is_include(const String& text);



		void _process_ifdef(PreprocessorOpCode opcode, SPreprocessorValue& value);

		void _process_include(PreprocessorOpCode opcode, SPreprocessorValue& value);

	};


}


#endif