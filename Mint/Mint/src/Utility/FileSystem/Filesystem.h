#ifndef _MINT_FILESYSTEM_H_
#define _MINT_FILESYSTEM_H_


#include "Path.h"
#include "FileIO.h"


namespace mint
{

	class CFilesystem
	{
	public:
		CFilesystem();
		CFilesystem(CPath starting_directory);
		CFilesystem(const CFilesystem& other);
		CFilesystem& operator=(const CFilesystem& other);

		static CPath construct_from(CPath path, const String& addition);

		static CPath construct_from(const String& path, const String& addition);

		static CPath get_working_directory();

		static bool set_working_directory(CPath path);

		static bool create_directory(CPath directory_path, const String& directory_name);

		static bool create_directory(CPath complete_directory_path);

		static bool delete_directory_or_file(CPath entry_path, const String& entry_full_name);

		static bool delete_directory_or_file(CPath complete_entry_path);

		static bool create_file(CPath directory_path, String& file_name, String& file_extension, bool add_dot_before_extension = true);

		static bool find_directory(CPath query_directory, const String& searched_directory_name);

		static bool find_file(CPath query_directory, const String& file_stem, const String& file_extension);

		static bool find_file_by_stem(CPath query_directory, const String& file_stem);

		static bool find_file_by_extension(CPath query_directory, const String& file_extension);

		static char* read_file_at_path(CPath complete_path, u32* out_file_size);

		static char* read_file_data_from_file_handle(FILE* file, u32 file_size);

		static CPath get_relative_path_to_working_directory(CPath complete_path);


		bool forward_brute_force(CPath path);

		bool forward_brute_force(const String& path);

		bool forward(CPath path);

		bool forward(const String& path);

		bool forward_pretend(CPath path);

		bool forward_pretend(const String& path);

		bool back();


		void append_path(CPath path);

		void append_path(const String& path);

		CPath get_current_directory();

		bool is_a_directory();

		bool is_a_file();

		Vector< CPath > get_all_entries_in_current_dir();

		Vector< CPath > get_all_directories_in_current_dir();

		Vector< CPath > get_all_files_in_current_dir();

		Vector< CPath > get_all_files_in_current_dir_by_extension(const String& extension);


		CPath get_full_path_of_file(const String& full_file_name);

		CPath get_full_path_of_first_file_by_extension(const String& file_extension);

		CPath get_full_path_of_first_file_by_stem(const String& file_stem);


		CPath get_full_path_of_directory(const String& directory);

	private:
		CPath m_currentPath;

	};

}


#endif