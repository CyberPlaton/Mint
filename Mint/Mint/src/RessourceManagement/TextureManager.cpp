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

		);
	}


	mint::Vec2 CTextureManager::get_texture_dimension(const String& texture_name)
	{
		return { 0.0f, 0.0f };
	}


	mint::Vec2 CTextureManager::get_texture_dimension(TextureHandle& handle)
	{
		return { 0.0f, 0.0f };
	}


	mint::TextureHandle CTextureManager::get_texture_handle(const String& texture_name)
	{
		return 0;
	}


}