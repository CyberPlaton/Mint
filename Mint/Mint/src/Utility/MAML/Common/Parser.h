#ifndef _MAML_PARSER_H_
#define _MAML_PARSER_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"

#include "../../Logging/Logging.h"

#include "Token.h"
#include "../Document.h"


namespace maml
{

	class CParser
	{
	public:
		CParser(const mint::Vector< SToken >& stream);

		bool parse(CDocument& document);


	private:
		CDocument* m_document;

		mint::u64 m_cursor;

		mint::Vector< SToken > m_tokens;

		SNode* m_currentNode;

		bool m_panik;

		mint::String m_errorMessage;


	private:

		void program();

		void node();

		void node_header();

		void node_body();

		void node_end();

		void properties();

		void property();

		void value(mint::CAny& property_data);

		void string(mint::CAny& data);

		void vsint(mint::CAny& data);

		void vuint(mint::CAny& data);

		void vfloat(mint::CAny& data);

		void vector2(mint::CAny& data);

		void vector3(mint::CAny& data);

		void vector4(mint::CAny& data);

		void vfalse(mint::CAny& data);

		void vtrue(mint::CAny& data);

		void array(mint::Vector< mint::CAny >& vec);

		void array_body(mint::Vector< mint::CAny >& vec);



		mint::f32 _to_float(const mint::String& text);

		mint::u64 _to_uint(const mint::String& text);

		mint::s64 _to_sint(const mint::String& text);

		bool _to_bool(const mint::String& text);

		mint::Vec2 _to_vector2(const mint::String& text);

		mint::Vec3 _to_vector3(const mint::String& text);

		mint::Vec4 _to_vector4(const mint::String& text);



		SToken& _get_current_token();

		SToken& _get_next_token();

		SToken::TokenType _peek_current();

		SToken::TokenType _peek_next();

		void _advance();

		bool _has_error();

		void _report_error(const mint::String& message);

		void _report_error(const mint::String& expected, const mint::String& got);
	};
}


#endif