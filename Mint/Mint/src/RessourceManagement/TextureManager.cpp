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


	bool CTextureManager::add_texture(const String& texture_name, bgfx::TextureHandle& handle, bgfx::TextureInfo& info)
	{
		MINT_ASSERT(bgfx::isValid(handle) == true, "Passing an invalid handle is a critical error!");

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

}