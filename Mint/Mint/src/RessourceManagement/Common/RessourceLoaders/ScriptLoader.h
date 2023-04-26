#ifndef _MINT_SCRIPT_LOADER_H_
#define _MINT_SCRIPT_LOADER_H_


#include "IRessourceLoader.h"

#include "Scripting/ScriptEngine.h"

namespace mint
{
	class CScriptLoader : public IRessourceLoader
	{
	public:
		MINT_DEFINE_RESSOURCE_LOADER_CTOR(CScriptLoader);

		bool load_ressource(const String& ressource_type, CAsset& asset) override final;

		CAsset load_asset(const String& ressource_container_folder, const String& ressource_type, maml::SNode* node) override final;

	};
}

#endif