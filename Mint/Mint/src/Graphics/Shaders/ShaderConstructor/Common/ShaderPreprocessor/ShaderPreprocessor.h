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
		PreprocessorOpCode_If,
		PreprocessorOpCode_Else,
		PreprocessorOpCode_Endif,
	};


	enum PreprocessorValueType
	{
		PreprocessorValueType_Invalid = -1,
		PreprocessorValueType_String = 0,
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
		u64 m_cursor = 0;

		u64 m_directiveBegin = 0;
		u64 m_directiveEnd = 0;

		String m_source;

		String m_shaderName;

		String m_shaderPath;

	private:
		String _parse_shader_file();
	
		bool _process_directive(PreprocessorOpCode opcode, SPreprocessorValue& value, String& error);


		void _step();

		char _advance();

		char _peek_current();

		char _peek_next();


		bool _is_newline(char c);

		bool _is_eof(char c);

		bool _is_whitespace(char c);

		bool _is_quote(char c);

		bool _is_preprocessor_directive(char c);

		bool _is_identifier(char c);


		PreprocessorOpCode _get_op_code(const String& text);
		SPreprocessorValue _get_preprocessor_value(const String& text);

		bool _is_directive_opengl_native(const String& text);
	};


}


#endif