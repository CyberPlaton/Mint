#ifndef _MINT_TEXTURE_MANAGER_H_
#define _MINT_TEXTURE_MANAGER_H_


#include "Common/Common.h"
#include "Utility/STL/Map.h"
#include "Common/Algorithm.h"
#include "Utility/Logging/Logging.h"


namespace mint
{
	class CTextureManager
	{
	public:
		STATIC_GET(CTextureManager, s_CTextureManager);

		bool initialize();

		void terminate();

		void reset();


		TextureHandle add_texture(const String& texture_name, Texture& texture);

		Vec2 get_texture_dimension(const String& texture_name);

		Vec2 get_texture_dimension(TextureHandle handle);

		TextureHandle get_texture_handle(const String& texture_name);

		const Vector< Texture >& get_all_textures();

		const Texture& get_texture(TextureHandle handle) const;


	private:
		static CTextureManager* s_CTextureManager;


		MINT_CRITICAL_SECTION(m_criticalSection);

		CMap< Texture > m_textures;
	};


};



#endif