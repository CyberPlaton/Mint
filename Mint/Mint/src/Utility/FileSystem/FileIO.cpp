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

				MINT_LOG_WARN("[{:.4f}][CFileReaderWriter::open] Opened FILE without closing \"{}\"!", MINT_APP_TIME, file_path);

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


	bool CFileReaderWriter::write_to_file_at_path(const String& file_path, void* data_buffer, u32 data_buffer_size)
	{
		bool result = false;

		std::ofstream out(file_path.c_str(), std::ios_base::out);

		if(out.is_open())
		{
			result = write_to_file_at_path(out, data_buffer, data_buffer_size);
		}
		else
		{
			const char* error = strerror(errno);
			MINT_LOG_ERROR("[{:.4f}][CFileReaderWriter::write_to_file_at_path] Failed writing to file \"{}\", reason: \"{}\"!", MINT_APP_TIME, file_path, error);
		}

		out.close();

		return result;
	}


	bool CFileReaderWriter::write_to_file_at_path(std::ofstream& stream, void* data_buffer, u32 data_buffer_size)
	{
		stream.write(reinterpret_cast< const char* >(data_buffer), data_buffer_size);
		
		// Closing and "writing" to file happens automatically
		// on std::ofstream destruction.
		return true;
	}


}