#include "FileIO.h"


namespace mint
{
	FILE* CFileReaderWriter::open(const String& file_path, uint32_t* file_size, AccessMode mode)
	{
		std::filesystem::directory_entry path(file_path.c_str());

		*file_size = 0;

		if (path.exists())
		{
			String m;
			FILE* file;
			errno_t result;

			switch (mode)
			{
			case AccessMode_Read:
				m = "r";
				break;
			case AccessMode_Write:
				m = "w";
				break;
			case AccessMode_Append:
				m = "a";
				break;
			case AccessMode_ReadWrite:
			default:
				m = "r+";
				break;
			}

			if (result = fopen_s(&file, file_path.c_str(), m.c_str()); result == 0)
			{
				*file_size = static_cast<uint32_t>(path.file_size());

				return file;
			}

			m.clear();
		}

		return nullptr;
	}


	uint32_t CFileReaderWriter::get_file_size(FILE* file)
	{
		uint32_t file_size = 0;

		fseek(file, 0, SEEK_END);

		file_size = ftell(file);

		fseek(file, 0, SEEK_SET);

		return file_size;
	}
}