#include "Lexer.h"

#include <string.h>

namespace maml
{


	CLexer::CLexer() : 
		m_cursor(0), m_currentLine(0), m_panik(false)
	{

	}


	mint::Vector< maml::SToken > CLexer::scan_from_file(const mint::String& maml_file_path)
	{
		mint::CFileystem fs(mint::CFileystem::get_working_directory());
		std::ifstream in;

		if(fs.forward(maml_file_path))
		{
			in.open(fs.get_current_directory().as_string().c_str());

			if (!in.is_open())
			{
				mint::String msg = "Could not open file at \"";
				msg.append(maml_file_path.c_str());
				msg.append("\! Message: ");

				std::string err = strerror(errno);
				msg.append(err.c_str());

				m_tokens.push_back(_make_error_token(msg));

				return m_tokens;
			}
		}

		std::string line;
		while(std::getline(in, line))
		{
			m_source += "\n";

			m_source += line;
		}

		in.close();


		return _begin_scan();
	}


	mint::Vector< SToken > CLexer::scan_from_source(const mint::String& maml_source)
	{
		m_source = maml_source;

		return _begin_scan();
	}


	mint::Vector< SToken > CLexer::_begin_scan()
	{
		_primary_scanning_pass();

		if(!m_panik)
		{
			m_source = "";
			m_cursor = 0;
			m_currentLine = 0;

			_secondary_scanning_pass();
		}

		return m_tokens;
	}

	void CLexer::_primary_scanning_pass()
	{
		for(;;)
		{
			SToken token;

			token = _get_next_token();


			if(token.m_type == SToken::TokenType_Error)
			{
				m_errorToken = token;
				m_panik = true;
				break;
			}
			if (token.m_type == SToken::TokenType_EOF)
			{
				break;
			}

			m_tokens.push_back(token);
		}
	}


	void CLexer::_secondary_scanning_pass()
	{
		for(m_cursor = 0; m_cursor < m_tokens.size(); m_cursor++)
		{
			const auto& token = m_tokens[m_cursor];

			if(_is_number(token.m_type))
			{
				if(_is_vectorn(4))
				{
					_make_vector_token_secondary_pass(4);
				}
				else if (_is_vectorn(3))
				{
					_make_vector_token_secondary_pass(3);
				}
				else if (_is_vectorn(2))
				{
					_make_vector_token_secondary_pass(2);
				}
			}
		}
	}


	maml::SToken CLexer::_get_next_token()
	{
		m_currentToken = "";

		char c = _peek_current();

		while(_is_whitespace(c))
		{
			if(_is_newline(c)) m_currentLine++;

			c = _advance();
		}

		if(_is_eof(c))
		{
			return _make_token(SToken::TokenType_EOF, "\0", m_currentLine);
		}
		if(_is_digit(c))
		{
			return _make_number_token();
		}
		if (_is_identifier(c))
		{
			return _make_identifier_token();
		}
		if (_is_comment(c))
		{
			_advance();

			while(_peek_current() != '\n')
			{
				c = _advance();
			}

			m_currentLine++;

			return _get_next_token();
		}



		switch(c)
		{
		case '=':
		{
			_advance();

			return _make_token(SToken::TokenType_Equal, "=", m_currentLine);
		}
		case ':':
		{
			_advance();

			return _make_token(SToken::TokenType_Colon, ":", m_currentLine);
		}
		case '"':
		{
			_advance();

			return _make_string_token();
		}
		case '[':
		{
			_advance();

			return _make_token(SToken::TokenType_Lbracket, "[", m_currentLine);
		}
		case ']':
		{
			_advance();

			return _make_token(SToken::TokenType_Rbracket, "]", m_currentLine);
		}
		case ',':
		{
			_advance();

			return _make_token(SToken::TokenType_Comma, ",", m_currentLine);
		}
		case '-':
		{
			_advance();

			m_currentToken = "-";

			return _make_number_token(true);
		}
		default: break;
		}


		return _make_error_token("Unrecognized character!");
	}


	maml::SToken& CLexer::_get_token_at(mint::u64 index)
	{
		return m_tokens[index];
	}

	bool CLexer::_is_vectorn(mint::u64 dimension)
	{
		if(_can_advance_secondary_pass(dimension))
		{
			bool accum = true;
			
			for(auto i = 0; i < dimension; i++)
			{
				accum &= _is_number(m_tokens[m_cursor + i].m_type);

				if (!accum) break;
			}

			return accum;
		}

		return false;
	}


	bool CLexer::_is_number(SToken::TokenType type)
	{
		return (type == SToken::TokenType_Float ||
				type == SToken::TokenType_Uint ||
				type == SToken::TokenType_Sint);
	}


	void CLexer::_make_vector_token_secondary_pass(mint::u64 dimension)
	{
		mint::String vstring;
		SToken::TokenType vtype = SToken::TokenType_Invalid;

		switch(dimension)
		{
		case 2:
		{
			vstring.append(_get_token_at(m_cursor).m_text.c_str());

			vstring.append(" ");

			vstring.append(_get_token_at(m_cursor + 1).m_text.c_str());

			vtype = SToken::TokenType_Vector2;
			break;
		}
		case 3:
		{
			vstring.append(_get_token_at(m_cursor).m_text.c_str());

			vstring.append(" ");

			vstring.append(_get_token_at(m_cursor + 1).m_text.c_str());

			vstring.append(" ");

			vstring.append(_get_token_at(m_cursor + 2).m_text.c_str());

			vtype = SToken::TokenType_Vector3;
			break;
		}
		case 4:
		{
			vstring.append(_get_token_at(m_cursor).m_text.c_str());

			vstring.append(" ");

			vstring.append(_get_token_at(m_cursor + 1).m_text.c_str());

			vstring.append(" ");

			vstring.append(_get_token_at(m_cursor + 2).m_text.c_str());

			vstring.append(" ");

			vstring.append(_get_token_at(m_cursor + 3).m_text.c_str());

			vtype = SToken::TokenType_Vector4;
			break;
		}
		default:
		{
			break;
		}
		}


		SToken token(vtype, vstring, _get_token_at(m_cursor).m_line);

		m_tokens[m_cursor] = std::move(token);

		_erase_tokens_secondary_pass(m_cursor + 1, dimension - 1);
	}


	maml::SToken CLexer::_make_number_token(bool is_negative /*= false*/)
	{
		std::string current_token;
		bool fpoint = false;

		while(_is_digit(_peek_current()))
		{
			current_token += _peek_current();

			_advance();
		}

		if(_peek_current() == '.')
		{
			fpoint = true;

			_advance();

			current_token += ".";

			while(_is_digit(_peek_current()))
			{
				current_token += _peek_current();

				_advance();
			}
		}


		if(fpoint)
		{
			m_currentToken.append(current_token.c_str());

			return _make_token(SToken::TokenType_Float, m_currentToken, m_currentLine);
		}

		if(is_negative)
		{
			m_currentToken.append(current_token.c_str());

			return _make_token(SToken::TokenType_Sint, m_currentToken, m_currentLine);
		}
		else
		{
			m_currentToken.append(current_token.c_str());

			return _make_token(SToken::TokenType_Uint, m_currentToken, m_currentLine);
		}
	}


	maml::SToken CLexer::_make_string_token()
	{
		std::string current_token;

		char c = _peek_current();

		while(c != '"' && !_is_eof(c) && c != '\n')
		{
			current_token += c;

			c = _advance();
		}

		if(_is_eof(c))
		{
			return _make_error_token("Unterminated String!");
		}
		if (_is_newline(c))
		{
			return _make_error_token("Multiline String!");
		}

		_advance();

		m_currentToken.append(current_token.c_str());

		return _make_token(SToken::TokenType_String, m_currentToken, m_currentLine);
	}


	maml::SToken CLexer::_make_identifier_token()
	{
		std::string current_token;

		char c = _peek_current();

		while(_is_identifier(c))
		{
			current_token += c;

			c = _advance();
		}


		m_currentToken = current_token;


		if(_is_end(current_token))
		{
			return _make_token(SToken::TokenType_End, m_currentToken, m_currentLine);
		}
		if (_is_false(current_token))
		{
			return _make_token(SToken::TokenType_False, m_currentToken, m_currentLine);
		}
		if (_is_true(current_token))
		{
			return _make_token(SToken::TokenType_True, m_currentToken, m_currentLine);
		}

		return _make_token(SToken::TokenType_Id, m_currentToken, m_currentLine);
	}


	maml::SToken CLexer::_make_error_token(const mint::String& message)
	{
		m_panik = true;

		return {SToken::TokenType_Error, message, m_currentLine};
	}


	maml::SToken CLexer::_make_token(SToken::TokenType type, const mint::String& text, mint::u64 line)
	{
		return { type, text, line };
	}


	void CLexer::_step()
	{
		m_cursor++;
	}


	void CLexer::_step_secondary_pass()
	{
		m_cursor++;
	}


	bool CLexer::_can_advance_secondary_pass(mint::u64 steps)
	{
		return m_cursor + steps < m_tokens.size();
	}


	char CLexer::_advance()
	{
		return m_source[++m_cursor];
	}


	SToken::TokenType CLexer::_advance_secondary_pass()
	{
		return m_tokens[++m_cursor].m_type;
	}


	char CLexer::_peek_current()
	{
		return m_source[m_cursor];
	}


	SToken::TokenType CLexer::_peek_current_secondary_pass()
	{
		return m_tokens[m_cursor].m_type;
	}


	char CLexer::_peek_next()
	{
		return m_source[m_cursor + 1];
	}


	SToken::TokenType CLexer::_peek_next_secondary_pass()
	{
		return m_tokens[m_cursor + 1].m_type;
	}


	void CLexer::_erase_tokens_secondary_pass(mint::u64 starting_index, mint::u64 count)
	{
		m_tokens.erase(m_tokens.begin() + starting_index,
					   m_tokens.begin() + starting_index + count);
	}


	bool CLexer::_is_eof(char character)
	{
		return character == '\0';
	}


	bool CLexer::_is_digit(char character)
	{
		return character >= '0' && character <= '9';
	}


	bool CLexer::_is_whitespace(char character)
	{
		return (character == '\n' || character == '\t' ||
				character == '\f' || character == '\r' ||
				character == ' ');
	}


	bool CLexer::_is_identifier(char character)
	{
		return ((character >= 'a' && character <= 'z') ||
				(character >= 'A' && character <= 'Z') ||
				character == '_');
	}


	bool CLexer::_is_newline(char character)
	{
		return character == '\n';
	}


	bool CLexer::_is_comment(char character)
	{
		return character == '#';
	}


	bool CLexer::_is_end(const mint::String& text)
	{
		return text == "end";
	}


	bool CLexer::_is_false(const mint::String& text)
	{
		return text == "False";
	}


	bool CLexer::_is_true(const mint::String& text)
	{
		return text == "True";
	}
}