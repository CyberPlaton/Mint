#include "Filesystem.h"


namespace mint
{


	CFileystem::CFileystem()
	{
	}


	CFileystem::CFileystem(CPath starting_directory) :
		m_currentPath(starting_directory)
	{
	}


	CFileystem::CFileystem(const CFileystem& other) :
		m_currentPath(other.m_currentPath)
	{
	}

	mint::CFileystem& CFileystem::operator=(const CFileystem& other)
	{
		m_currentPath = other.m_currentPath;

		return *this;
	}

	mint::CPath CFileystem::construct_from(CPath path, const String& addition)
	{
		CPath p(path);

		p.append(addition);

		return p;
	}


	mint::CPath CFileystem::construct_from(const String& path, const String& addition)
	{
		CPath p(path);

		p.append(addition);

		return p;
	}


	mint::CPath CFileystem::get_working_directory()
	{
		return std::filesystem::current_path();
	}


	bool CFileystem::set_working_directory(CPath path)
	{
		if(path.does_exist() && path.is_directory())
		{
			std::filesystem::current_path(path.as_path());

			return true;
		}

		return false;
	}


	bool CFileystem::create_directory(CPath directory_path, const String& directory_name)
	{
		CFileystem fs(directory_path);

		fs.get_current_directory().operator/=(directory_name);

		if(!CFileystem::find_directory(directory_path, directory_name))
		{
			return std::filesystem::create_directory(fs.get_current_directory().as_path());
		}

		return true;
	}


	bool CFileystem::create_directory(CPath complete_directory_path)
	{
		if(!complete_directory_path.does_exist())
		{
			return std::filesystem::create_directory(complete_directory_path.as_path());
		}

		return true;
	}


	bool CFileystem::create_file(CPath directory_path, String& file_name, String& file_extension)
	{
		if(directory_path.does_exist())
		{
			String name;
			name.append(file_name.c_str());
			name.append(file_extension.c_str());

			directory_path /= name;

			if(directory_path.does_exist())
			{
				std::ofstream out(directory_path.as_string().c_str());

				out.close();

				return true;
			}
		}

		return false;
	}


	bool CFileystem::find_directory(CPath query_directory, const String& searched_directory_name)
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


	bool CFileystem::find_file(CPath query_directory, const String& file_stem, const String& file_extension)
	{
		if (query_directory.does_exist())
		{
			CPath searched(query_directory);

			searched.append(file_stem);
			searched.append(file_extension);


			for (auto& entry : std::filesystem::directory_iterator{ searched.as_path() })
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


	bool CFileystem::find_file_by_stem(CPath query_directory, const String& file_stem)
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


	bool CFileystem::find_file_by_extension(CPath query_directory, const String& file_extension)
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


	bool CFileystem::forward_brute_force(CPath path)
	{
		CPath p = get_current_directory();

		p /= path;

		if(!p.does_exist())
		{
			if (!CFileystem::create_directory(p)) return false;
		}

		m_currentPath = p;

		return true;
	}


	bool CFileystem::forward_brute_force(const String& path)
	{
		CPath p(path);

		return forward_brute_force(p);
	}


	bool CFileystem::forward(CPath path)
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


	bool CFileystem::forward(const String& path)
	{
		CPath p(path);

		return forward(p);
	}


	bool CFileystem::back()
	{
		if(m_currentPath.as_path().has_parent_path())
		{
			m_currentPath = m_currentPath.as_path().parent_path();

			return true;
		}

		return false;
	}


	void CFileystem::append_path(CPath path)
	{
		m_currentPath /= path;
	}


	void CFileystem::append_path(const String& path)
	{
		m_currentPath /= path;
	}


	mint::CPath CFileystem::get_current_directory()
	{
		return m_currentPath;
	}


	bool CFileystem::is_a_directory()
	{
		return m_currentPath.is_directory();
	}


	bool CFileystem::is_a_file()
	{
		return m_currentPath.is_file();
	}


	Vector< mint::CPath > CFileystem::get_all_entries_in_current_dir()
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


	Vector< mint::CPath > CFileystem::get_all_directories_in_current_dir()
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


	Vector< mint::CPath > CFileystem::get_all_files_in_current_dir()
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


	mint::CPath CFileystem::get_full_path_of_file(const String& full_file_name)
	{
		for(auto& path: get_all_files_in_current_dir())
		{
			if (path.as_string() == full_file_name) return path /= full_file_name;
		}

		return mint::CPath{};
	}


	mint::CPath CFileystem::get_full_path_of_first_file_by_extension(const String& file_extension)
	{
		for (auto& path : get_all_files_in_current_dir())
		{
			if (path.get_extension() == file_extension) return path;
		}

		return mint::CPath{};
	}


	mint::CPath CFileystem::get_full_path_of_first_file_by_stem(const String& file_stem)
	{
		for (auto& path : get_all_files_in_current_dir())
		{
			if (path.get_stem() == file_stem) return path;
		}

		return mint::CPath{};
	}


	mint::CPath CFileystem::get_full_path_of_directory(const String& directory)
	{
		for (auto& path : get_all_directories_in_current_dir())
		{
			if (path.as_string() == directory) return path;
		}

		return mint::CPath{};
	}


}