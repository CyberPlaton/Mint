#ifndef _MAML_TOKEN_H_
#define _MAML_TOKEN_H_


#include "Common/Common.h"


namespace maml
{

	struct SToken
	{
		enum TokenType
		{
			TokenType_Invalid = 0,

			TokenType_EOF,
			TokenType_Error,

			TokenType_Id,
			TokenType_Equal,
			TokenType_Colon,
			TokenType_Comma,
			TokenType_Lbracket,
			TokenType_Rbracket,
			TokenType_End,
			TokenType_False,
			TokenType_True,
			TokenType_Float,
			TokenType_Uint,
			TokenType_Sint,
			TokenType_String,
			TokenType_Vector2,
			TokenType_Vector3,
			TokenType_Vector4
		};


		SToken() :
			m_type(TokenType_Invalid), m_text(""), m_line(0)
		{
		}

		SToken(TokenType type, const mint::String text, mint::u64 line) :
			m_type(type), m_text(text), m_line(line)
		{
		}


		mint::u64 m_line;

		mint::String m_text;

		TokenType m_type;
	};
}

#endif