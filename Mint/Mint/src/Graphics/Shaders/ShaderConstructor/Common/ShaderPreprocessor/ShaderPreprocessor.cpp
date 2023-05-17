#include "ShaderPreprocessor.h"


namespace mint::fx::sc
{


	String CShaderPreprocessor::parse_shader_file(const String& file_path)
	{
		CFileystem fs(CFileystem::get_working_directory());


		if(fs.forward(file_path))
		{
			u32 size = 0;
			auto file = CFileReaderWriter::open(fs.get_current_directory().as_string(), &size);
		
			if(!file)
			{
				MINT_LOG_ERROR("[{:.4f}][CShaderPreprocessor::parse_shader_file] Failed opening Shader file at \"{}\"!", MINT_APP_TIME, fs.get_current_directory().as_string());
				return false;
			}

			auto data = CFileystem::read_file_data_from_file_handle(file, size);

			if(!data)
			{
				MINT_LOG_ERROR("[{:.4f}][CShaderPreprocessor::parse_shader_file] Failed reading data from Shader file at \"{}\"!", MINT_APP_TIME, fs.get_current_directory().as_string());
				return false;
			}


			m_shaderName = fs.get_current_directory().get_stem() + fs.get_current_directory().get_extension();
			
			fs.back();
			m_shaderPath = fs.get_current_directory().as_string();
				
			return _parse_shader_code_part(String(data));
		}

		return "";
	}


	mint::fx::sc::SPreprocessorValue CShaderPreprocessor::_get_preprocessor_value(const String& text)
	{
		// What we expect is the following format:
		// [preprocessor directive][opcode][whitespace][value], i.e.
		//            #              ifdef    ' '       DEBUG

		SPreprocessorValue pvalue;
		pvalue.m_type = PreprocessorValueType_Invalid;

		// Get the index where the whitespace starts and ignore it.
		u64 index = text.find_first_of(' ') + 1;
		
		// Get the text value from the index to the end.
		String value = text.substr(index);

		mint::algorithm::string_to_lower(value);


		// Process the value and extract its type.
		u64 first_quote = text.find_first_of('"');
		u64 last_quote = text.find_last_of('"');

		if(first_quote != std::string::npos &&
		   last_quote != std::string::npos &&
		   first_quote != last_quote)
		{
			pvalue.m_type = PreprocessorValueType_String;

			// Extract the pure text value ignoring the quotes.
			String s;
			s = value.substr(1, value.size() - 2);

			pvalue.m_value = CAny(s);
		}
		else
		{
			pvalue.m_type = PreprocessorValueType_Identifier;
			pvalue.m_value = CAny(value);
		}

		return pvalue;
	}


	mint::fx::sc::PreprocessorOpCode CShaderPreprocessor::_get_op_code(const String& text)
	{
		if(text.find("#include") != std::string::npos)
		{
			return PreprocessorOpCode_Include;
		}
		else if (text.find("#ifdef") != std::string::npos)
		{
			return PreprocessorOpCode_Ifdef;
		}
		else if (text.find("#else") != std::string::npos)
		{
			return PreprocessorOpCode_Else;
		}
		else if (text.find("#endif") != std::string::npos)
		{
			return PreprocessorOpCode_Endif;
		}
		
		return PreprocessorOpCode_Invalid;
	}



	bool CShaderPreprocessor::_get_next_line(std::istringstream& stream, String& output_string)
	{
		return std::getline(stream, output_string).good();
	}

	mint::String CShaderPreprocessor::_parse_shader_code_part(String& shader_code, u64 line_start /*= 0*/, u64 line_end /*= UINT_MAX*/)
	{
		std::istringstream ss(shader_code);

		while(_get_next_line(ss, m_currentLine))
		{
			// Ignore the lines before the starting line.
			if (m_currentLineNumber < line_start) continue;

			// Ignore the lines after the ending line.
			if (m_currentLineNumber > line_end) break;


			// Process the current line.

			// Check whether the line contains a preprocessor directive.
			if(_does_line_have_preprocessor_directive(m_currentLine))
			{
				if(_is_include(m_currentLine))
				{
					PreprocessorOpCode opcode = _get_op_code(m_currentLine);
					SPreprocessorValue value = _get_preprocessor_value(m_currentLine);

					_process_include(opcode, value);

					// Do not include the current line, because its the preprocessor directive
					// not supported by OpenGL compiler!
					continue;
				}
				if (_is_ifdef(m_currentLine))
				{
					PreprocessorOpCode opcode = _get_op_code(m_currentLine);
					SPreprocessorValue value = _get_preprocessor_value(m_currentLine);

					_process_ifdef(opcode, value);

					continue;
				}
			}

			// Append the source line by line like it was defined.
			m_finalSource += m_currentLine + "\n";

			m_currentLineNumber++;
		}

		return m_finalSource;
	}


	bool CShaderPreprocessor::_does_line_have_preprocessor_directive(const String& text)
	{
		if(text[0] == '#')
		{
			if(text.find("version") != std::string::npos)
			{
				// Ignore the OpenGL native directive.
				return false;
			}

			// Do not ignore out own directives.
			return true;
		}

		return false;
	}


	bool CShaderPreprocessor::_is_ifdef(const String& text)
	{
		return text.find("#ifdef") != std::string::npos;
	}


	bool CShaderPreprocessor::_is_else(const String& text)
	{
		return text.find("#else") != std::string::npos;
	}


	bool CShaderPreprocessor::_is_endif(const String& text)
	{
		return text.find("#endif") != std::string::npos;
	}


	bool CShaderPreprocessor::_is_include(const String& text)
	{
		return text.find("#include") != std::string::npos;
	}


	void CShaderPreprocessor::_process_include(PreprocessorOpCode opcode, SPreprocessorValue& value)
	{
		// Construct filepath for shader to be included.
		CFileystem fs(m_shaderPath);

		if(!fs.forward(value.m_value.cast< String >()))
		{
			// File not found.
		}


		CShaderPreprocessor pp;
		String source_to_include = pp.parse_shader_file(fs.get_current_directory().as_string());

		// Add the included source to current source where it should have been included.
		m_finalSource += source_to_include;
	}


	void CShaderPreprocessor::_process_ifdef(PreprocessorOpCode opcode, SPreprocessorValue& value)
	{

	}


}