#include "EditorIconManager.h"


namespace mint::editor
{

	mint::editor::CEditorIconManager* CEditorIconManager::s_CEditorIconManager = nullptr;



	const mint::Texture& CEditorIconManager::get_texture(const String& texture_name)
	{
		auto h = mint::algorithm::djb_hash(texture_name);

		return get_texture(h);
	}


	const mint::Texture& CEditorIconManager::get_texture(TextureHandle texture_handle)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const auto & texture = m_textures.get_const(texture_handle);

		);

		return texture;
	}


	mint::Vec2 CEditorIconManager::get_texture_dimension(const String& texture_name)
	{
		auto h = mint::algorithm::djb_hash(texture_name);

		return get_texture_dimension(h);
	}


	mint::Vec2 CEditorIconManager::get_texture_dimension(TextureHandle handle)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			const auto & texture = m_textures.get_const(handle);

		);

		return { texture.GetWidth(), texture.GetHeight() };
	}


	bool CEditorIconManager::initialize(const String& icon_ressources_path /*= mint::editor::s_EditorIconsPath*/)
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		bool result = true;

		mint::CFileystem fs(mint::CFileystem::get_working_directory());

		if(fs.forward(icon_ressources_path) &&
		   fs.forward("Icons.maml"))
		{
			maml::CDocument document(MAML_DOCUMENT_SIZE_BIG);
			maml::SNode* root = nullptr;

			if(root = CSerializer::load_maml_document(fs.get_current_directory().as_string(), document); root != nullptr)
			{
				for(auto& property : maml::CDocument::get_all_node_properties(root))
				{
					MINT_ASSERT(property.is< mint::String >() == true, "Invalid data type for icon name provided! Names must be \"strings\" only.");

					auto icon_editor_name = property.get_property_name();
					auto icon_name = property.cast< mint::String >();

					try
					{
						mint::CFileystem icon_file_path(fs.get_current_directory());

						if(icon_file_path.forward(icon_name))
						{
							mint::Texture texture(icon_file_path.get_current_directory().as_string());

							auto h = mint::algorithm::djb_hash(icon_editor_name);

							m_textures.add(h, texture);
						}
						else
						{
							MINT_LOG_ERROR("[{:.4f}][CEditorIconManager::initialize] Failed loading editor icon  \"{}\"!", MINT_APP_TIME, icon_name);
						}
					}
					catch (const raylib::RaylibException& e)
					{
					}
				}
			}
		}
		else
		{

			MINT_LOG_CRITICAL("[{:.4f}][CEditorIconManager::initialize] Failed loading editor icons at \"{}\"!", MINT_APP_TIME, fs.get_current_directory().as_string());

			result = false;
		}

		return result;
	}


	void CEditorIconManager::terminate()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_textures.reset();

		);

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


}