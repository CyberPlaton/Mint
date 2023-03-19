#include "Path.h"


namespace mint
{


	CPath::CPath(const CPath& other)
	{
		m_path = other.m_path;
		m_directory = other.m_directory;
	}

	mint::CPath& CPath::operator=(const CPath& other)
	{
		m_path = other.m_path;
		m_directory = other.m_directory;

		return *this;
	}

	String CPath::as_string()
	{
		return m_path.string().c_str();
	}


	std::filesystem::path CPath::as_path()
	{
		return m_path;
	}


	std::filesystem::directory_entry CPath::as_dir()
	{
		return std::filesystem::directory_entry{ m_path };
	}


	const bool CPath::does_exist()
	{
		return m_directory.exists();
	}


	String CPath::get_extension()
	{
		return m_path.extension().string().c_str();
	}


	String CPath::get_stem()
	{
		return m_path.stem().string().c_str();
	}


	bool CPath::has_extension(const String& string)
	{
		return get_extension() == string;
	}


	bool CPath::is_directory()
	{
		if (does_exist()) return m_directory.is_directory();

		return false;
	}


	bool CPath::is_file()
	{
		if (does_exist()) return m_directory.is_regular_file();

		return false;
	}


	void CPath::append(const CPath& other)
	{
		this->operator/=(other);
	}


	void CPath::append(const String& path)
	{
		this->operator/=(path);
	}


	bool CPath::operator==(const String& other)
	{
		return m_path == std::filesystem::path(other.c_str());
	}


	bool CPath::operator==(const std::filesystem::path& other)
	{
		return m_path == other;
	}


	bool CPath::operator==(const CPath& other)
	{
		return m_path == other.m_path;
	}


	mint::CPath& CPath::operator/=(const String& other)
	{
		CPath path(other);

		return this->operator/=(path);
	}


	mint::CPath& CPath::operator/=(const CPath& other)
	{
		m_path /= other.m_path;

		m_directory = std::filesystem::directory_entry(m_path);

		return *this;
	}


}