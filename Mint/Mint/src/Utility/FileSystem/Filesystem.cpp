#include "Filesystem.h"


namespace mint
{


	CFilesystem::CFilesystem()
	{
	}


	CFilesystem::CFilesystem(CPath starting_directory) :
		m_currentPath(starting_directory)
	{
	}


	CFilesystem::CFilesystem(const CFilesystem& other) :
		m_currentPath(other.m_currentPath)
	{
	}

	mint::CFilesystem& CFilesystem::operator=(const CFilesystem& other)
	{
		m_currentPath = other.m_currentPath;

		return *this;
	}

	mint::CPath CFilesystem::construct_from(CPath path, const String& addition)
	{
		CPath p(path);

		p.append(addition);

		return p;
	}


	mint::CPath CFilesystem::construct_from(const String& path, const String& addition)
	{
		CPath p(path);

		p.append(addition);

		return p;
	}


	mint::CPath CFilesystem::get_working_directory()
	{
		return std::filesystem::current_path();
	}


	bool CFilesystem::set_working_directory(CPath path)
	{
		if(path.does_exist() && path.is_directory())
		{
			std::filesystem::current_path(path.as_path());

			return true;
		}

		return false;
	}


	bool CFilesystem::create_directory(CPath directory_path, const String& directory_name)
	{
		CFilesystem fs(directory_path);

		fs.get_current_directory().operator/=(directory_name);

		if(!CFilesystem::find_directory(directory_path, directory_name))
		{
			return std::filesystem::create_directory(fs.get_current_directory().as_path());
		}

		return true;
	}


	bool CFilesystem::create_directory(CPath complete_directory_path)
	{
		if(!complete_directory_path.does_exist())
		{
			return std::filesystem::create_directory(complete_directory_path.as_path());
		}

		return true;
	}


	bool CFilesystem::delete_directory_or_file(CPath entry_path, const String& entry_full_name)
	{
		CFilesystem fs(entry_path);

		fs.get_current_directory().operator/=(entry_full_name);

		return delete_directory_or_file(fs.get_current_directory());
	}


	bool CFilesystem::delete_directory_or_file(CPath complete_entry_path)
	{
		std::error_code ec;

		if (complete_entry_path.does_exist())
		{
			auto result = std::filesystem::remove_all(complete_entry_path.as_path(), ec);

			return result > 0; // result == 1 means we deleted the folder/file, result > 1 means we deleted the folder with a bunch of files/folders.
		}

		return false;
	}


	bool CFilesystem::create_file(CPath directory_path, const String& file_name, const String& file_extension, bool add_dot_before_extension)
	{
		if(directory_path.does_exist())
		{
			String name;
			name.append(file_name.c_str());
			if(add_dot_before_extension) name.append(".");
			name.append(file_extension.c_str());

			directory_path /= name;

			if(!directory_path.does_exist())
			{
				std::ofstream out(directory_path.as_string().c_str());

				std::filesystem::permissions(directory_path.as_string().c_str(),
											 std::filesystem::perms::owner_all 
										   | std::filesystem::perms::group_all,
											 std::filesystem::perm_options::add);
				out.close();

				return true;
			}
		}

		return false;
	}


	bool CFilesystem::find_directory(CPath query_directory, const String& searched_directory_name)
	{
		if(query_directory.does_exist())
		{
			CPath searched(query_directory);

			searched.append(searched_directory_name);

			for(auto& entry : std::filesystem::directory_iterator{ searched.as_path() })
			{
				CPath path(entry);

				if(path.is_directory() && path == searched)
				{
					return true;
				}
			}
		}

		return false;
	}


	bool CFilesystem::find_file(CPath query_directory, const String& file_stem, const String& file_extension)
	{
		if (query_directory.does_exist())
		{
			CPath searched(query_directory);

			searched.append(file_stem + file_extension);

			for (auto& entry : std::filesystem::directory_iterator{ query_directory.as_path() })
			{
				CPath path(entry);

				if (path.is_file() && path == searched)
				{
					return true;
				}
			}
		}

		return false;
	}


	bool CFilesystem::find_file_by_stem(CPath query_directory, const String& file_stem)
	{
		if (query_directory.does_exist())
		{
			for (auto& entry : std::filesystem::directory_iterator{ query_directory.as_path() })
			{
				CPath path(entry);

				if (path.is_file() && path.get_stem() == file_stem)
				{
					return true;
				}
			}
		}

		return false;
	}


	bool CFilesystem::find_file_by_extension(CPath query_directory, const String& file_extension)
	{
		if (query_directory.does_exist())
		{
			for (auto& entry : std::filesystem::directory_iterator{ query_directory.as_path() })
			{
				CPath path(entry);

				if (path.is_file() && path.get_extension() == file_extension)
				{
					return true;
				}
			}
		}

		return false;
	}


	char* CFilesystem::read_file_at_path(CPath complete_path, u32* out_file_size)
	{
		FILE* file = NULL;
		u32 file_size = 0;

		if(file = CFileReaderWriter::open(complete_path.as_string(), &file_size);
		   file != nullptr)
		{
			// Intercept edge case of empty file.
			if (file_size == 0)
			{
				void* buffer = malloc(256);

				std::memset(buffer, '\0', 256);

				*out_file_size = 256;

				fclose(file);

				MINT_LOG_WARN("[{:.4f}][CFilesystem::read_file_at_path] Closed opened FILE \"{}\"!", MINT_APP_TIME, complete_path.as_string());

				return reinterpret_cast<char*>(buffer);
			}


			*out_file_size = file_size;

			auto out_data = read_file_data_from_file_handle(file, file_size);

			fclose(file);

			MINT_LOG_WARN("[{:.4f}][CFilesystem::read_file_at_path] Closed opened FILE \"{}\"!", MINT_APP_TIME, complete_path.as_string());

			return out_data;
		}

		return nullptr;
	}


	char* CFilesystem::read_file_data_from_file_handle(FILE* file, u32 file_size)
	{
		if (file != nullptr && file_size > 0)
		{
			auto file_size = CFileReaderWriter::get_file_size(file);

			void* buffer = malloc(file_size);

			std::memset(buffer, '\0', file_size);

			if (auto read_bytes = fread(buffer, sizeof(char), file_size, file);
				read_bytes > 0)
			{
				return reinterpret_cast<char*>(buffer);
			}

			free(buffer);
		}

		return nullptr;
	}


	mint::CPath CFilesystem::get_relative_path_to_working_directory(CPath complete_path)
	{
		CPath wdir_absolute = std::filesystem::absolute(CFilesystem::get_working_directory().as_path());

		return std::filesystem::relative(complete_path.as_path(), wdir_absolute.as_path());
	}


	mint::CPath CFilesystem::get_file_parent_directory_relative_to_working_directory(CPath complete_file_path)
	{
		CFilesystem fs(CFilesystem::get_relative_path_to_working_directory(complete_file_path));

		fs.back();

		return fs.get_current_directory();
	}

	bool CFilesystem::forward_brute_force(CPath path)
	{
		CPath p = get_current_directory();

		p /= path;

		if(!p.does_exist())
		{
			if (!CFilesystem::create_directory(p)) return false;
		}

		m_currentPath = p;

		return true;
	}


	bool CFilesystem::forward_brute_force(const String& path)
	{
		CPath p(path);

		return forward_brute_force(p);
	}


	bool CFilesystem::forward(CPath path)
	{
		CPath p = m_currentPath;

		p /= path;

		if(p.does_exist())
		{
			m_currentPath = p;

			return true;
		}

		return false;
	}


	bool CFilesystem::forward(const String& path)
	{
		CPath p(path);

		return forward(p);
	}


	bool CFilesystem::forward_pretend(CPath path)
	{
		CPath p(path);

		return forward_pretend(p);
	}


	bool CFilesystem::forward_pretend(const String& path)
	{
		CPath p = m_currentPath;

		p /= path;

		if (p.does_exist())
		{
			return true;
		}

		return false;
	}


	bool CFilesystem::back()
	{
		if(m_currentPath.as_path().has_parent_path())
		{
			m_currentPath = m_currentPath.as_path().parent_path();

			return true;
		}

		return false;
	}


	void CFilesystem::append_path(CPath path)
	{
		m_currentPath /= path;
	}


	void CFilesystem::append_path(const String& path)
	{
		m_currentPath /= path;
	}


	mint::CPath CFilesystem::get_current_directory()
	{
		return m_currentPath;
	}


	bool CFilesystem::is_a_directory()
	{
		return m_currentPath.is_directory();
	}


	bool CFilesystem::is_a_file()
	{
		return m_currentPath.is_file();
	}


	Vector< mint::CPath > CFilesystem::get_all_entries_in_current_dir()
	{
		if(is_a_directory())
		{
			Vector< CPath > entries;

			for (const auto& path : std::filesystem::directory_iterator{ m_currentPath.as_path() })
			{
				entries.emplace_back( path );
			}

			return entries;
		}

		return Vector< CPath >{};
	}


	Vector< mint::CPath > CFilesystem::get_all_directories_in_current_dir()
	{
		if (is_a_directory())
		{
			Vector< CPath > entries;

			for (const auto& path : std::filesystem::directory_iterator{ m_currentPath.as_path() })
			{
				std::filesystem::directory_entry query(path);

				if(query.is_directory())
				{
					entries.emplace_back(path);
				}
			}

			return entries;
		}

		return Vector< CPath >{};
	}


	Vector< mint::CPath > CFilesystem::get_all_files_in_current_dir()
	{
		if (is_a_directory())
		{
			Vector< CPath > entries;

			for (const auto& path : std::filesystem::directory_iterator{ m_currentPath.as_path() })
			{
				std::filesystem::directory_entry query(path);

				if (query.is_regular_file())
				{
					entries.emplace_back(path);
				}
			}

			return entries;
		}

		return Vector< CPath >{};
	}


	mint::Vector< mint::CPath > CFilesystem::get_all_files_in_current_dir_by_extension(const String& extension)
	{
		if (is_a_directory())
		{
			Vector< CPath > entries;

			for (const auto& path : std::filesystem::directory_iterator{ m_currentPath.as_path() })
			{
				std::filesystem::directory_entry query(path);

				if (query.is_regular_file() && query.path().extension() == extension)
				{
					entries.emplace_back(path);
				}
			}

			return entries;
		}

		return Vector< CPath >{};
	}

	mint::CPath CFilesystem::get_full_path_of_file(const String& full_file_name)
	{
		for(auto& path: get_all_files_in_current_dir())
		{
			if (path.as_string() == full_file_name) return path /= full_file_name;
		}

		return mint::CPath{};
	}


	mint::CPath CFilesystem::get_full_path_of_first_file_by_extension(const String& file_extension)
	{
		for (auto& path : get_all_files_in_current_dir())
		{
			if (path.get_extension() == file_extension) return path;
		}

		return mint::CPath{};
	}


	mint::CPath CFilesystem::get_full_path_of_first_file_by_stem(const String& file_stem)
	{
		for (auto& path : get_all_files_in_current_dir())
		{
			if (path.get_stem() == file_stem) return path;
		}

		return mint::CPath{};
	}


	mint::CPath CFilesystem::get_full_path_of_directory(const String& directory)
	{
		for (auto& path : get_all_directories_in_current_dir())
		{
			if (path.as_string() == directory) return path;
		}

		return mint::CPath{};
	}


}