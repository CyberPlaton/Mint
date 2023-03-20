#ifndef _MAML_LEXER_H_
#define _MAML_LEXER_H_


#include "Common/Common.h"

#include "Utility/FileSystem/Filesystem.h"

#include "Token.h"

#include <string>
#include <fstream>


namespace maml
{
	class CLexer
	{
	public:
		CLexer();

		mint::Vector< SToken > scan_from_file(const mint::String& maml_file_path);

		mint::Vector< SToken > scan_from_source(const mint::String& maml_source);

		
	private:
		mint::String m_source;

		mint::String m_currentToken;

		mint::Vector< SToken > m_tokens;

		mint::u64 m_cursor;

		mint::u64 m_currentLine;

		bool m_panik;

		SToken m_errorToken;

	private:
		mint::Vector< SToken > _begin_scan();

		void _primary_scanning_pass();

		void _secondary_scanning_pass();

		SToken _get_next_token();


		SToken& _get_token_at(mint::u64 index);

		bool _is_vectorn(mint::u64 dimension);

		bool _is_number(SToken::TokenType type);


		
		void _make_vector_token_secondary_pass(mint::u64 dimension);

		SToken _make_number_token(bool is_negative = false);

		SToken _make_string_token();

		SToken _make_identifier_token();

		SToken _make_error_token(const mint::String& message);

		SToken _make_token(SToken::TokenType type, const mint::String& text, mint::u64 line);



		void _step();

		void _step_secondary_pass();

		bool _can_advance_secondary_pass(mint::u64 steps);

		char _advance();

		SToken::TokenType _advance_secondary_pass();

		char _peek_current();

		SToken::TokenType _peek_current_secondary_pass();

		char _peek_next();

		SToken::TokenType _peek_next_secondary_pass();

		void _erase_tokens_secondary_pass(mint::u64 starting_index, mint::u64 count);



		bool _is_eof(char character);

		bool _is_digit(char character);

		bool _is_whitespace(char character);

		bool _is_identifier(char character);

		bool _is_newline(char character);

		bool _is_comment(char character);

		bool _is_end(const mint::String& text);

		bool _is_false(const mint::String& text);

		bool _is_true(const mint::String& text);
	};
}

#endif