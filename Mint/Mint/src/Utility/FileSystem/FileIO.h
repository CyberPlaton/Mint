#ifndef _MINT_FILEIO_H_
#define _MINT_FILEIO_H_


#include "Common/Common.h"
#include "Utility/Logging/Logging.h"
#include <filesystem>
#include <fstream>


namespace mint
{
	class CFileReaderWriter
	{
	public:
		enum AccessMode
		{
			AccessMode_ReadWrite = 0,
			AccessMode_Read,
			AccessMode_Write,
			AccessMode_Append,
		};

	public:
		static FILE* open(const String& file_path, uint32_t* file_size, AccessMode mode = AccessMode_ReadWrite);

		static uint32_t get_file_size(FILE* file);

		static bool write_to_file_at_path(const String& file_path, void* data_buffer, u32 data_buffer_size);

		static bool write_to_file_at_path(std::ofstream& stream, void* data_buffer, u32 data_buffer_size);
	};
}

#endif