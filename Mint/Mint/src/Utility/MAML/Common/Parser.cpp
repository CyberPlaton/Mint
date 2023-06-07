#include "Parser.h"


namespace maml
{




	CParser::CParser(const mint::Vector< SToken >& stream) : 
		m_tokens(stream), m_document(nullptr), m_cursor(0), m_currentNode(nullptr), m_panik(false)
	{
		if(!m_tokens.empty())
		{
			if(m_tokens.begin()->m_type == SToken::TokenType_Error)
			{
				m_panik = true;
				m_errorMessage = m_tokens.begin()->m_text;
			}
		}
	}


	bool CParser::parse(CDocument& document)
	{
		if(m_tokens.empty() || m_panik)
		{
			MINT_LOG_ERROR("[{:.4f}][CParser::parse] Error while scanning the document: \"{}\"", MINT_APP_TIME, m_errorMessage);
			return false;
		}

		m_document = &document;

		m_currentNode = m_document->get_root();


		program();


		if(m_panik)
		{
			MINT_LOG_ERROR("[{:.4f}][CParser::parse] Error while parsing the document: \"{}\"", MINT_APP_TIME, m_errorMessage);

			return false;
		}
		return true;
	}


	void CParser::program()
	{
		node();
	}


	void CParser::node()
	{
		node_header();
		node_body();
		node_end();
	}


	void CParser::node_header()
	{
		if(_peek_current() == SToken::TokenType_Id)
		{
			if(_peek_next() == SToken::TokenType_Colon)
			{
				auto node = m_document->create_node(_get_current_token().m_text, m_currentNode);

				m_currentNode = node;

				_advance(); _advance();

				return;
			}

			_report_error(":", _get_next_token().m_text);
		}

		_report_error("valid identifier as name of the node", _get_current_token().m_text);
	}


	void CParser::node_body()
	{
		properties();
	}


	void CParser::node_end()
	{
		if(_peek_current() == SToken::TokenType_End)
		{
			if(m_currentNode->m_parent)
			{
				m_currentNode = m_currentNode->m_parent;
			}

			_advance();

			return;
		}

		_report_error("\"end\" keyword as scope end for a Node", _get_current_token().m_text);
	}


	void CParser::properties()
	{
		while(_peek_current() == SToken::TokenType_Id && !_has_error())
		{
			property();
		}
	}


	void CParser::property()
	{
		if(_peek_next() == SToken::TokenType_Equal)
		{
			SProperty property;

			property.m_name = _get_current_token().m_text;

			_advance(); _advance();


			mint::CAny data;

			value(data);

			property.m_data = std::move(data);


			m_document->add_property_to_node(m_currentNode, property);

			return;
		}
		
		if (_peek_next() == SToken::TokenType_Colon)
		{
			node();

			return;
		}

		_report_error("\"end\" keyword as scope end for a Node", _get_current_token().m_text);
	}


	void CParser::value(mint::CAny& property_data)
	{
		switch(_peek_current())
		{
		case SToken::TokenType_Uint:
		{
			vuint(property_data);
			break;
		}
		case SToken::TokenType_Sint:
		{
			vsint(property_data);
			break;
		}
		case SToken::TokenType_Float:
		{
			vfloat(property_data);
			break;
		}
		case SToken::TokenType_String:
		{
			string(property_data);
			break;
		}
		case SToken::TokenType_Vector2:
		{
			vector2(property_data);
			break;
		}
		case SToken::TokenType_Vector3:
		{
			vector3(property_data);
			break;
		}
		case SToken::TokenType_Vector4:
		{
			vector4(property_data);
			break;
		}
		case SToken::TokenType_False:
		{
			vfalse(property_data);
			break;
		}
		case SToken::TokenType_True:
		{
			vtrue(property_data);
			break;
		}
		case SToken::TokenType_Lbracket:
		{
			mint::Vector< mint::CAny > vec;

			array(vec);

			property_data.set(vec);

			break;
		}
		default:
		{
			mint::String message = "Unexpected value type provided for token: \""; message += _get_current_token().m_text; message += "\"";
			_report_error(message);
			break;
		}
		}
	}


	void CParser::string(mint::CAny& data)
	{
		data.set(_get_current_token().m_text);

		_advance();
	}


	void CParser::vsint(mint::CAny& data)
	{
		data.set(_to_sint(_get_current_token().m_text));

		_advance();
	}


	void CParser::vuint(mint::CAny& data)
	{
		data.set(_to_uint(_get_current_token().m_text));

		_advance();
	}


	void CParser::vfloat(mint::CAny& data)
	{
		data.set(_to_float(_get_current_token().m_text));

		_advance();
	}


	void CParser::vector2(mint::CAny& data)
	{
		data.set(_to_vector2(_get_current_token().m_text));

		_advance();
	}


	void CParser::vector3(mint::CAny& data)
	{
		data.set(_to_vector3(_get_current_token().m_text));

		_advance();
	}


	void CParser::vector4(mint::CAny& data)
	{
		data.set(_to_vector4(_get_current_token().m_text));

		_advance();
	}


	void CParser::vfalse(mint::CAny& data)
	{
		data.set(_to_bool(_get_current_token().m_text));

		_advance();
	}


	void CParser::vtrue(mint::CAny& data)
	{
		data.set(_to_bool(_get_current_token().m_text));

		_advance();
	}


	void CParser::array(mint::Vector< mint::CAny >& vec)
	{
		_advance();

		array_body(vec);

		if(_peek_current() == SToken::TokenType_Rbracket)
		{
			_advance();

			return;
		}

		_report_error("\"]\" to close the array", _get_current_token().m_text);
	}


	void CParser::array_body(mint::Vector< mint::CAny >& vec)
	{
		if(_peek_current() == SToken::TokenType_Rbracket)
		{
			return;
		}



		mint::CAny data;

		value(data);

		vec.push_back(data);



		while(_peek_current() == SToken::TokenType_Comma)
		{
			_advance();

			mint::CAny data;

			value(data);

			vec.push_back(data);
		}


		if(_peek_current() != SToken::TokenType_Rbracket)
		{
			_report_error("\",\" to separate values in the array or \"]\" to close it", _get_current_token().m_text);
		}
	}


	mint::f32 CParser::_to_float(const mint::String& text)
	{
		mint::f32 dvalue = mint::algorithm::string_to_generic_number< mint::f32 >(text);

		return dvalue;
	}


	mint::u64 CParser::_to_uint(const mint::String& text)
	{
		return SCAST(mint::u64, std::stoull(text.c_str(), nullptr));
	}


	mint::s64 CParser::_to_sint(const mint::String& text)
	{
		return SCAST(mint::s64, std::stoll(text.c_str(), nullptr));
	}


	bool CParser::_to_bool(const mint::String& text)
	{
		return text == mint::String("True");
	}


	mint::Vec2 CParser::_to_vector2(const mint::String& text)
	{
		mint::Vector< mint::String > numbers;

		mint::algorithm::string_split(text, ' ', numbers);

		return { _to_float(numbers[0]), _to_float(numbers[1]) };
	}


	mint::Vec3 CParser::_to_vector3(const mint::String& text)
	{
		mint::Vector< mint::String > numbers;

		mint::algorithm::string_split(text, ' ', numbers);

		return { _to_float(numbers[0]), _to_float(numbers[1]), _to_float(numbers[2]) };
	}


	mint::Vec4 CParser::_to_vector4(const mint::String& text)
	{
		mint::Vector< mint::String > numbers;

		mint::algorithm::string_split(text, ' ', numbers);

		return { _to_float(numbers[0]), _to_float(numbers[1]), _to_float(numbers[2]), _to_float(numbers[3]) };
	}


	maml::SToken& CParser::_get_current_token()
	{
		return m_tokens[m_cursor];
	}


	maml::SToken& CParser::_get_next_token()
	{
		return m_tokens[m_cursor + 1];
	}


	SToken::TokenType CParser::_peek_current()
	{
		return m_tokens[m_cursor].m_type;
	}


	SToken::TokenType CParser::_peek_next()
	{
		return m_tokens[m_cursor + 1].m_type;
	}


	void CParser::_advance()
	{
		m_cursor++;
	}


	bool CParser::_has_error()
	{
		return m_panik;
	}


	void CParser::_report_error(const mint::String& message)
	{
		m_errorMessage.append(message.c_str());
		m_panik = true;
	}


	void CParser::_report_error(const mint::String& expected, const mint::String& got)
	{
		m_errorMessage = "Expected a \"";

		m_errorMessage += expected;

		m_errorMessage += "\" after symbol \"";

		m_errorMessage += _get_current_token().m_text;

		m_errorMessage += "\" at line " + std::to_string(_get_current_token().m_line);

		m_errorMessage += ", instead got \"";

		m_errorMessage += got;

		m_errorMessage += "\"!";

		m_panik = true;
	}


}