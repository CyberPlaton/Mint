#include "TextureManager.h"

namespace mint
{

	mint::CTextureManager* CTextureManager::s_CTextureManager = nullptr;


	bool CTextureManager::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}


	void CTextureManager::terminate()
	{
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CTextureManager::reset()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_textures.reset();
			m_textureInfo.reset();

		);
	}


	bool CTextureManager::add_texture(const String& texture_name, TextureHandle& handle, bgfx::TextureInfo& info)
	{
		MINT_ASSERT(bgfx::isValid(handle) == true, "Passing an invalid handle!");

		auto h = mint::algorithm::djb_hash(texture_name);


		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			bool result = m_textures.lookup(h);
			result |= m_textureInfo.lookup(h);
				
		);


		if(result)
		{
			MINT_LOG_WARN("[{:.4f}][CTextureManager::add_texture] Attempt to add a duplicate Texture \"{}\"!", MINT_APP_TIME, texture_name.c_str());
			return false;
		}

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_textures.add(h, handle);
			m_textureInfo.add(h, info);

		);

		return true;
	}


	mint::Vec2 CTextureManager::get_texture_dimension(const String& texture_name)
	{
		auto h = mint::algorithm::djb_hash(texture_name);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const bool result = m_textureInfo.lookup(h);

		);

		if(result)
		{
			Vec2 vec;

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				const auto& info = m_textureInfo.get(h);

				vec.x = SCAST(f32, info.width);
				vec.y = SCAST(f32, info.height);

			);


			return vec;
		}

		MINT_LOG_WARN("[{:.4f}][CTextureManager::get_texture_dimension] Requested Texture \"{}\" was not found!", MINT_APP_TIME, texture_name.c_str());
		return { 0, 0 };
	}


	mint::TextureHandle CTextureManager::get_texture_handle(const String& texture_name)
	{
		auto h = mint::algorithm::djb_hash(texture_name);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const bool result = m_textures.lookup(h);

		);

		if(result)
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				const auto& handle = m_textures.get(h);

			);

			return handle;
		}

		MINT_LOG_WARN("[{:.4f}][CTextureManager::get_texture_handle] Requested Texture \"{}\" was not found!", MINT_APP_TIME, texture_name.c_str());
		return BGFX_INVALID_HANDLE;
	}


}