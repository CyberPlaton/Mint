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
				
			m_source = String(data);
			
			return _parse_shader_file();
		}

		return "";
	}


	String CShaderPreprocessor::_parse_shader_file()
	{
		if (m_source.empty()) return "";

		char c = m_source[0];

		while(!_is_eof(c))
		{
			if(_is_preprocessor_directive(c))
			{
				c = _advance();

				// Get the op code.
				String opcode;

				// Mark the start of the preprocessor directive, including the '#' sign.
				m_directiveBegin = m_cursor - 1;
				
				while(!_is_whitespace(c))
				{
					opcode += c;

					c = _advance();
				}

				// Consume whitespaces.
				c = _advance();
				while(_is_whitespace(c))
				{
					c = _advance();
				}

				String value;
				// Get the value for the op code.
				while(!_is_newline(c))
				{
					value += c;

					c = _advance();
				}

				// Mark the start of the preprocessor directive, including the '\n' sign.
				m_directiveEnd = m_cursor + 1;


				PreprocessorOpCode oc = _get_op_code(opcode);
				SPreprocessorValue v = _get_preprocessor_value(value);
				
				if(v.m_type != PreprocessorValueType_Invalid)
				{
					// Erase the directive from the parsed source to be valid for OpenGL compiler.
					// Note: We erase before proceeding, as we might have to include another shader file, thus 
					// making the directive begin and end marks invalid.
					mint::algorithm::string_erase_range(m_source, m_directiveBegin, m_directiveEnd);



					String error;
					if (!_process_directive(oc, v, error))
					{
						MINT_LOG_ERROR("[{:.4f}][CShaderPreprocessor::_parse_shader_file] Error parsing preprocessor directive \"{}\" and value \"{}\". Error message: \"{}\"!", MINT_APP_TIME, opcode, value, error);
						return "";
					}
				}

				m_directiveBegin = 0;
				m_directiveEnd = 0;
			}

			c = _advance();
		}

		return m_source;
	}


	char CShaderPreprocessor::_advance()
	{
		return m_source[++m_cursor];
	}


	char CShaderPreprocessor::_peek_current()
	{
		return m_source[m_cursor];
	}


	char CShaderPreprocessor::_peek_next()
	{
		return m_source[m_cursor + 1];
	}


	void CShaderPreprocessor::_step()
	{
		m_cursor++;
	}


	mint::fx::sc::SPreprocessorValue CShaderPreprocessor::_get_preprocessor_value(const String& text)
	{
		SPreprocessorValue pvalue;
		pvalue.m_type = PreprocessorValueType_Invalid;

		u64 first_quote = text.find_first_of('"');
		u64 last_quote = text.find_last_of('"');

		if(first_quote != std::string::npos &&
		   last_quote != std::string::npos &&
		   first_quote != last_quote)
		{
			pvalue.m_type = PreprocessorValueType_String;

			// Extract the pure value ignoring the quotes.
			String s;
			s = text.substr(1, text.size() - 2);

			pvalue.m_value = CAny(s);
		}

		return pvalue;
	}


	mint::fx::sc::PreprocessorOpCode CShaderPreprocessor::_get_op_code(const String& text)
	{
		if(text == "include")
		{
			return PreprocessorOpCode_Include;
		}
		else if (text == "if")
		{
			return PreprocessorOpCode_If;
		}
		else if (text == "else")
		{
			return PreprocessorOpCode_Else;
		}
		else if (text == "endif")
		{
			return PreprocessorOpCode_Endif;
		}
		
		return PreprocessorOpCode_Invalid;
	}


	bool CShaderPreprocessor::_process_directive(PreprocessorOpCode opcode, SPreprocessorValue& value, String& error)
	{
		if(opcode == PreprocessorOpCode_Invalid)
		{
			error = "Unrecognized preprocessor directive";
			return false;
		}
		if(value.m_type == PreprocessorValueType_Invalid)
		{
			error = "Unexpected preprocessor value";
			return false;
		}


		switch(opcode)
		{
		case PreprocessorOpCode_Include:
		{
			CShaderPreprocessor pp;
			String include_shader;

			CFileystem fs(m_shaderPath);

			// Get the shader path to be included into the current source.
			if(!fs.forward(value.m_value.cast< String >()))
			{
				error = "Unable to find shader file at \"" + fs.get_current_directory().as_string() + "\"";
				return false;
			}

			// Check for recursive inclusion.
			if(fs.get_current_directory().as_string() == m_shaderPath)
			{
				error = "Recursive self-inclusion detected in shader \"" + fs.get_current_directory().as_string() + "\"";
				return false;
			}

			// Get the shader source to be included into the current source.
			include_shader = pp.parse_shader_file(fs.get_current_directory().as_string());
			if(include_shader.empty())
			{
				error = "Failed parsing shader file \"" + fs.get_current_directory().as_string() + "\" for \"" + m_shaderPath + "\"";
				return false;
			}

			mint::algorithm::string_push_front(m_source, include_shader);

			m_cursor += include_shader.size();

			return true;
			break;
		}
		case PreprocessorOpCode_If:
		{
			break;
		}
		case PreprocessorOpCode_Else:
		{
			break;
		}
		case PreprocessorOpCode_Endif:
		{
			break;
		}
		}

		return false;
	}


	bool CShaderPreprocessor::_is_newline(char c)
	{
		return c == '\n';
	}


	bool CShaderPreprocessor::_is_eof(char c)
	{
		return c == '\0';
	}


	bool CShaderPreprocessor::_is_whitespace(char c)
	{
		return (c == '\n' || c == '\t' ||
				c == '\f' || c == '\r' ||
				c == ' ');
	}


	bool CShaderPreprocessor::_is_preprocessor_directive(char c)
	{
		return c == '#';
	}


	bool CShaderPreprocessor::_is_identifier(char c)
	{
		return ((c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				c == '_');
	}


	bool CShaderPreprocessor::_is_quote(char c)
	{
		return c == '"';
	}


	bool CShaderPreprocessor::_is_directive_opengl_native(const String& text)
	{
		if(text == "version")
		{
			return true;
		}

		return false;
	}


}