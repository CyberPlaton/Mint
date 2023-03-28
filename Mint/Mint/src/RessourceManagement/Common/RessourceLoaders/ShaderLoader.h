#ifndef _MINT_SHADER_LOADER_H_
#define _MINT_SHADER_LOADER_H_


#include "IRessourceLoader.h"

#include "../../ShaderManager.h"


namespace mint
{
	class CShaderLoader : public IRessourceLoader
	{
	public:
		MINT_DEFINE_RESSOURCE_LOADER_CTOR(CShaderLoader);

		bool load_ressource(const String& ressource_type, CAsset& asset) override final;

		CAsset load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node) override final;
	};
}

#endif