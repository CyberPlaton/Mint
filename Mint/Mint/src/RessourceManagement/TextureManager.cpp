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
		auto h = mint::algorithm::djb_hash(texture_name);

		const auto& texture = m_textures.get_const(h);

		return { texture.GetWidth(), texture.GetHeight() };
	}


	mint::Vec2 CTextureManager::get_texture_dimension(TextureHandle handle)
	{
		const auto& texture = m_textures.get_const(handle);

		return { texture.GetWidth(), texture.GetHeight() };
	}


	mint::TextureHandle CTextureManager::get_texture_handle(const String& texture_name)
	{
		return mint::algorithm::djb_hash(texture_name);
	}


	const mint::Vector< raylib::Texture >& CTextureManager::get_all_textures()
	{
		return m_textures.get_all_const();
	}


	const raylib::Texture& CTextureManager::get_texture(TextureHandle handle) const
	{
		return m_textures.get_const(handle);
	}


}