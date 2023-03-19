#ifndef _MINT_PATH_H_
#define _MINT_PATH_H_


#include "Common/Common.h"

#include <filesystem>

namespace mint
{

	class CPath
	{
	public:
		CPath() = default;
		CPath(const CPath& other);
		CPath& operator=(const CPath& other);
		CPath(const String & path) : m_path(path.c_str())
		{
			m_directory = std::filesystem::directory_entry(m_path);
		}
		CPath(std::filesystem::path path) : m_path(path)
		{
			m_directory = std::filesystem::directory_entry(m_path);
		}
		CPath(std::filesystem::directory_entry path) : m_path(path)
		{
			m_directory = std::filesystem::directory_entry(m_path);
		}


		String as_string();

		std::filesystem::path as_path();

		std::filesystem::directory_entry as_dir();

		const bool does_exist();

		String get_extension();

		String get_stem();

		bool has_extension(const String& string);

		bool is_directory();

		bool is_file();

		void append(const CPath& other);

		void append(const String& path);

		CPath& operator /=(const CPath& other);
		CPath& operator /=(const String& other);

		bool operator==(const CPath& other);
		bool operator==(const std::filesystem::path& other);
		bool operator==(const String& other);


	private:
		std::filesystem::path m_path;

		std::filesystem::directory_entry m_directory;
	};

}


#endif