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

		
		bool add_texture(const String& texture_name, bgfx::TextureHandle& handle, bgfx::TextureInfo& info);


	private:
		static CTextureManager* s_CTextureManager;


		MINT_CRITICAL_SECTION(m_criticalSection);

		CMap< bgfx::TextureHandle > m_textures;

		CMap< bgfx::TextureInfo > m_textureInfo;
	};
};



#endif