#include "EditorInternationalization.h"


namespace mint::editor::lang
{

	void set_used_language(Language language)
	{
		s_Language = language;
	}


	const char* get_word(Word word)
	{
		switch(s_Language)
		{
		case Language_German: return get_word_german(word);
		case Language_Russian: return get_word_russian(word);
		case Language_English: return get_word_english(word);
		}
	}


	const char* get_word_english(Word word)
	{
		switch (word)
		{
		case Word_Hierarchy: return u8"Hierarchy";
		}
	}


	const char* get_word_german(Word word)
	{
		switch (word)
		{
		case Word_Hierarchy: return u8"Hierarchie";
		}
	}


	const char* get_word_russian(Word word)
	{
		switch (word)
		{
		case Word_Hierarchy: return u8"Иерархия";
		}
	}


}