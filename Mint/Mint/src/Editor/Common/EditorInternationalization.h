#ifndef _MINTEDITOR_INTERNATIONALIZATION_H_
#define _MINTEDITOR_INTERNATIONALIZATION_H_



namespace mint::editor::lang
{
	enum Language
	{
		Language_English,
		Language_German,
		Language_Russian,
	};

	enum Word
	{
		Word_Hierarchy,
	};

	static Language s_Language = Language_English;

	void set_used_language(Language language);

	const char* get_word(Word word);

	const char* get_word_english(Word word);

	const char* get_word_german(Word word);

	const char* get_word_russian(Word word);

}


#endif