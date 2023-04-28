#ifndef _MINTEDITOR_EDITOR_ICON_MANAGER_H_
#define _MINTEDITOR_EDITOR_ICON_MANAGER_H_


#include "Common/Common.h"
#include "../Common/EditorCommon.h"
#include "Common/Algorithm.h"
#include "Utility/STL/Map.h"
#include "Utility/FileSystem/Filesystem.h"
#include "Utility/Logging/Logging.h"
#include "Utility/Serialization/Serializer.h"


namespace mint::editor
{

	class CEditorIconManager
	{
	public:
		STATIC_GET(CEditorIconManager, s_CEditorIconManager);

		bool initialize(const String& icon_ressources_path = mint::editor::s_EditorIconsPath);

		void terminate();


		const mint::Texture& get_texture(const String& texture_name);

		const mint::Texture& get_texture(TextureHandle texture_handle);

		Vec2 get_texture_dimension(const String& texture_name);

		Vec2 get_texture_dimension(TextureHandle handle);


	private:
		static CEditorIconManager* s_CEditorIconManager;

		MINT_CRITICAL_SECTION(m_criticalSection);

		mint::CMap< mint::Texture > m_textures;

	};
}



#endif