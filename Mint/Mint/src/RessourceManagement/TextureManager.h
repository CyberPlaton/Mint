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

		
		bool add_texture(const String& texture_name, TextureHandle& handle, bgfx::TextureInfo& info);

		Vec2 get_texture_dimension(const String& texture_name);

		TextureHandle get_texture_handle(const String& texture_name);


	private:
		static CTextureManager* s_CTextureManager;


		MINT_CRITICAL_SECTION(m_criticalSection);

		CMap< TextureHandle > m_textures;

		CMap< bgfx::TextureInfo > m_textureInfo;
	};
};



#endif