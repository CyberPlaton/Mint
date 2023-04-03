#include "Common.h"


namespace mint::fx
{

	bgfx::ShaderHandle loadShader(const String& name) {
		char* data = nullptr;
		std::ifstream file;
		size_t fileSize = 0;
		file.open(name.c_str(), std::ios::binary);
		if (file.is_open()) {
			file.seekg(0, std::ios::end);
			fileSize = static_cast<size_t>(file.tellg());
			data = new char[fileSize + 1];
			file.seekg(0, std::ios::beg);
			file.read(data, fileSize);
			file.close();

			auto dataRelease = [](void* ptr, void* userdata)
			{
				char* data = (char*)userdata;
				delete[] data;
			};
			const bgfx::Memory* mem = bgfx::makeRef(data, static_cast<uint32_t>(fileSize) + 1, dataRelease, data);
			mem->data[mem->size - 1] = '\0';
			bgfx::ShaderHandle handle = bgfx::createShader(mem);
			bgfx::setName(handle, name.c_str());
			return handle;
		}
		return { bgfx::kInvalidHandle };
	}


	bgfx::ProgramHandle loadProgram(const String& vs, const String& fs)
	{
		return bgfx::createProgram(loadShader(vs.c_str()), loadShader(fs.c_str()), true);
	}


	void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
	{
		bx::Error error;

		if (bx::open(_reader, _filePath, &error)) {
			uint32_t size = (uint32_t)bx::getSize(_reader);
			void* data = BX_ALLOC(_allocator, size);
			bx::read(_reader, data, size, bx::ErrorAssert{});
			bx::close(_reader);
			if (NULL != _size) {
				*_size = size;
			}
			return data;
		}
		
		if (NULL != _size) {
			*_size = 0;
		}

		return NULL;
	}

	void* load(const char* _filePath, uint32_t* _size)
	{
		return load(entry::getFileReader(), entry::getAllocator(), _filePath, _size);
	}

	void unload(void* _ptr)
	{
		BX_FREE(entry::getAllocator(), _ptr);
	}

	void imageReleaseCb(void* _ptr, void* _userData)
	{
		BX_UNUSED(_ptr);
		bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
		bimg::imageFree(imageContainer);
	}

	bgfx::TextureHandle loadTexture(bx::FileReaderI* _reader, const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
	{
		BX_UNUSED(_skip);
		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

		uint32_t size;
		void* data = load(_reader, entry::getAllocator(), _filePath, &size);
		if (NULL != data)
		{
			bimg::ImageContainer* imageContainer = bimg::imageParse(entry::getAllocator(), data, size);

			if (NULL != imageContainer) {
				if (NULL != _orientation) {
					*_orientation = imageContainer->m_orientation;
				}

				const bgfx::Memory* mem = bgfx::makeRef(
					imageContainer->m_data
					, imageContainer->m_size
					, imageReleaseCb
					, imageContainer
				);
				unload(data);

				if (imageContainer->m_cubeMap) {
					handle = bgfx::createTextureCube(
						uint16_t(imageContainer->m_width)
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}
				else if (1 < imageContainer->m_depth) {
					handle = bgfx::createTexture3D(
						uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, uint16_t(imageContainer->m_depth)
						, 1 < imageContainer->m_numMips
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}
				else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), _flags)) {
					handle = bgfx::createTexture2D(
						uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}

				if (bgfx::isValid(handle)) {
					bgfx::setName(handle, _filePath);
				}

				if (NULL != _info) {
					bgfx::calcTextureSize(
						*_info
						, uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, uint16_t(imageContainer->m_depth)
						, imageContainer->m_cubeMap
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
					);
				}
			}
		}

		return handle;
	}

	bgfx::TextureHandle loadTexture(const char* _name, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
	{
		return loadTexture(entry::getFileReader(), _name, _flags, _skip, _info, _orientation);
	}

	void setPlatformData(void* platformData)
	{
		entry::s_platformData = *(bgfx::PlatformData*)platformData;
	}

}


namespace entry
{

	bool FileReader::open(const bx::FilePath& _filePath, bx::Error* _err)
	{
		bxString filePath(s_currentDir);
		filePath.append(_filePath);
		return super::open(filePath.getPtr(), _err);
	}


	bool FileWriter::open(const bx::FilePath& _filePath, bool _append, bx::Error* _err)
	{
		bxString filePath(s_currentDir);
		filePath.append(_filePath);
		return super::open(filePath.getPtr(), _append, _err);
	}


	void setCurrentDir(const char* _dir)
	{
		s_currentDir.set(_dir);
	}


	void init()
	{
		s_fileReader = BX_NEW(g_allocator, FileReader);
		s_fileWriter = BX_NEW(g_allocator, FileWriter);
	}


	void terminate()
	{
		BX_DELETE(g_allocator, s_fileReader);
		s_fileReader = nullptr;
		BX_DELETE(g_allocator, s_fileWriter);
		s_fileWriter = nullptr;
	}


	bx::FileReaderI* getFileReader()
	{
		return s_fileReader;
	}


	bx::FileWriterI* getFileWriter()
	{
		return s_fileWriter;
	}


	bx::AllocatorI* getAllocator()
	{
		return g_allocator;
	}


}